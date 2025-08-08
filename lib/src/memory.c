#include "memory.h"

#include <stdlib.h>
#include <string.h>

#define FILESYS_LBLOCK  0 
#define FILESYS_BLOCKS  0 
#define FILESYS_TBLOCK  sizeof (uint16_t)
#define FILESYS_BITMAP  sizeof (uint16_t) + sizeof (uint8_t)

#define METADATA_FILENAME   0 
#define METADATA_BLOCKIN    64
#define METADATA_FILESIZE   64 + sizeof (uint8_t)
#define METADATA_DATETIME   64 + sizeof (uint8_t) + sizeof (uint32_t)

#define DATA_START  METADATA_DATETIME + sizeof (time_t)


static byte memory_disk[MEMORY_SIZE];
static flash_memory_map_st disk_access;

void memory_init(void)
{
    byte *memory_disk_ptr = memory_disk;
    flash_memory_filesystem_st fsys_header;

    memset(memory_disk, 0xff, sizeof (memory_disk));
    disk_access.entry_ptr = memory_disk;
    fsys_header.block_size = BLOCK_SIZE;
    fsys_header.total_blocks = BLOCK_CNT;
    fsys_header.free_bitmap = 0b11111110; // All blocks are suposed to be available except block 0 (reserved for filesystem)

    for (size_t block_id = 0; block_id < BLOCK_CNT; ++block_id)
    {
        for(size_t page_id = 0; page_id < PAGE_CNT; ++page_id)
        {
            disk_access.blocks[block_id].page_ptr[page_id] = memory_disk_ptr;
            memory_disk_ptr += PAGE_SIZE; // Equivalent to ((block id * total blocks) + page id) + page size
        }
    }

    // Writes the filesystem on memory
    memcpy(
            disk_access.blocks[FILESYS_LBLOCK].page_ptr[0],
            &fsys_header,
            sizeof (flash_memory_filesystem_st));
}

int memory_erase_block(size_t block_id)
{
    uint8_t lb_bitmap = 0b0;
    uint8_t free_bitmap = 0b0;

    if (block_id <= 0 || block_id >= BLOCK_CNT)
        return -1; // Out of bounds or trying to erase filesystem

    memset(disk_access.blocks[block_id].page_ptr[0], 0xff, BLOCK_SIZE);

    // Update the bitmap
    memcpy(&free_bitmap, disk_access.blocks[FILESYS_LBLOCK].page_ptr[0] + FILESYS_BITMAP, sizeof(uint8_t));
    lb_bitmap = 1 << block_id;
    free_bitmap &= lb_bitmap;
    memcpy(disk_access.blocks[FILESYS_LBLOCK].page_ptr[0], &free_bitmap, sizeof (uint8_t));

    return 0;
}

int flash_memory_write(char *filename, void *data, size_t data_len)
{
    byte *page_entry_ptr = NULL;
    uint8_t free_bitmap = 0b0;
    uint8_t lb_bitmap = 0b0;
    size_t block_id;
    size_t cnt_used_blocks = 0;
    size_t total_size = 0;
    size_t last_used_block = 0;
    flash_memory_dir_st dir_entry;

    memcpy(&free_bitmap, disk_access.blocks[0].page_ptr[0] + 4, sizeof (uint8_t));

    if (free_bitmap == 0)
        return -1; // No logical blocks available

    for (block_id = 0; (free_bitmap & 1) == 0; ++block_id);
    
    // Determine how many blocks will be used
    total_size = sizeof (flash_memory_dir_st) + data_len;
    
    while (total_size > 0)
    {
        total_size /= BLOCK_SIZE;
        ++cnt_used_blocks;

        // Prepare the bitmap update
        lb_bitmap <<= 1;
        lb_bitmap &= 1;
    }

    last_used_block = block_id + cnt_used_blocks;

    if (last_used_block > BLOCK_CNT)
        return -2; // No space available for the operation

    // Update the bitmap with the blocks that will be occupied
    lb_bitmap <<= last_used_block;
    free_bitmap ^= lb_bitmap;

    // Create the file metadata
    memset(&dir_entry, 0xff, sizeof(flash_memory_dir_st));
    memcpy(dir_entry.filename, filename, strlen(filename));
    dir_entry.block_in = block_id;
    dir_entry.file_size = data_len;
    dir_entry.datetime_created = time(NULL);

    // Copy metadata and data to the block
    page_entry_ptr = disk_access.blocks[block_id].page_ptr[0];
    memcpy(page_entry_ptr, &dir_entry, sizeof(dir_entry));
    memcpy(page_entry_ptr + sizeof (dir_entry), data, data_len);

    return 0; // Success
}

int flash_memory_get_str_filename(byte *dir_filename, char *out)
{
    size_t chr;

    if (dir_filename == NULL || out == NULL)
        return -1;

    for (chr = 0; chr < FILENAME_SIZE && dir_filename[chr] != 0xff; ++chr)
    {
        out[chr] = dir_filename[chr];
    }

    out[chr] = '\0';

    return 0;
}

int flash_memory_read(flash_memory_dir_st **out)
{
    static flash_memory_dir_st dir_entries[BLOCK_CNT - 1];

    if (*disk_access.entry_ptr == 0xff)
        return -1; // Flash card still erased

    // Ignore the block 0 which is the filesystem block
    for (size_t block_id = 1; block_id < BLOCK_CNT; ++block_id)
    {
        byte *page_entry_ptr = disk_access.blocks[block_id - 1].page_ptr[0];
        memcpy(dir_entries[block_id - 1].filename, page_entry_ptr, FILENAME_SIZE);
        page_entry_ptr += FILENAME_SIZE;
        memcpy(&dir_entries[block_id - 1].block_in, page_entry_ptr, sizeof (uint8_t));
        page_entry_ptr += sizeof (uint8_t);
        memcpy(&dir_entries[block_id - 1].file_size, page_entry_ptr, sizeof (uint32_t));
        page_entry_ptr += sizeof (uint32_t);
        memcpy(&dir_entries[block_id - 1].datetime_created, page_entry_ptr, sizeof (time_t));
    }

    *out = dir_entries;

    return 0;
}

int flash_memory_read_file(const char *filename, byte **out)
{
    flash_memory_dir_st dir_entry;

    if (filename == NULL || strlen(filename) > FILENAME_MAX)
        return -1; // Invalid name

    // From logical block 1, search for file name
    for (size_t block_id = 1; block_id < BLOCK_CNT; ++block_id)
    {
        char dir_filename[FILENAME_SIZE+1] = { '\0' };

        memcpy(&dir_entry, disk_access.blocks[block_id].page_ptr[0], sizeof (flash_memory_dir_st));

        flash_memory_get_str_filename(dir_entry.filename, dir_filename);

        if (strncmp(filename, dir_filename, strlen(filename)) == 0)
        {
            *out = calloc(dir_entry.file_size, sizeof (byte)); // Allocate memory as the file size is only acknowledged at read time
            if (*out == NULL)
                return -2; // Could not allocate enough memory 

            memcpy(*out, disk_access.blocks[block_id].page_ptr[0] + DATA_START, dir_entry.file_size);

            return 0; // Success
        }
    }

    return -3; // Requested file is not present on disk
}

void flash_memory_show_info(void)
{
    uint8_t free_bitmap = 0b0;
    size_t cnt_used_blocks;
    flash_memory_dir_st *dir_entries;

    flash_memory_read(&dir_entries);
    memcpy(&free_bitmap, disk_access.blocks[FILESYS_LBLOCK].page_ptr + FILESYS_BITMAP, sizeof(uint8_t));

    for (size_t i = 0; free_bitmap > 0; ++i)
    {
        cnt_used_blocks += free_bitmap & 0b0; // This results to "1" each time a bit is 0
        free_bitmap >>= 1;
    }

    printf("Flash disk info:\n");
    printf("Total size = %u (%u total blocks)\n", MEMORY_SIZE, BLOCK_CNT);
    printf("Occupied = %lu\n", cnt_used_blocks * BLOCK_SIZE);
    printf("\nroot:\n");

    for (size_t dir_entry_n = 0; dir_entry_n < BLOCK_CNT - 1; ++dir_entry_n)
    {
        // Read the first piece of data of the metadata, if it is "0xff" means no data is present at block
        if (dir_entries[dir_entry_n].filename[0] != 0xff)
        {
            char datetime_created_str[16] = { '\0' };
            char filename_str[FILENAME_SIZE+1] = { '\0' };
            time_t datetime_created = dir_entries[dir_entry_n].datetime_created;
            struct tm *datetime_created_st;

            datetime_created_st = localtime(&datetime_created);
            strftime(datetime_created_str, 16, "%b\t%d %H:%M", datetime_created_st);
            flash_memory_get_str_filename(dir_entries[dir_entry_n].filename, filename_str);
            printf("└─── %u\t%s\t%s\n", dir_entries[dir_entry_n].file_size, datetime_created_str, filename_str);
        }
    }
}
