#include "memory.h"

#include <string.h>


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
            disk_access.blocks[0].page_ptr[0],
            &fsys_header.block_size,
            sizeof (fsys_header.block_size));
    memcpy(
            disk_access.blocks[0].page_ptr[0] + sizeof(fsys_header.block_size),
            &fsys_header.total_blocks,
            sizeof (fsys_header.total_blocks));
    memcpy(
            disk_access.blocks[0].page_ptr[0] + sizeof(fsys_header.block_size) + sizeof (fsys_header.total_blocks),
            &fsys_header.free_bitmap,
            sizeof (fsys_header.free_bitmap));
}

int memory_erase_block(size_t block_id)
{
    if (block_id <= 0 || block_id >= BLOCK_CNT)
        return -1; // Out of bounds or trying to erase filesystem

    memset(disk_access.blocks[block_id].page_ptr[0], 0xff, BLOCK_SIZE);

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
        ++cnt_used_blocks;
        lb_bitmap <<= 1; // Prepare the bitmap update
        total_size /= BLOCK_SIZE;
    }

    last_used_block = block_id + cnt_used_blocks;

    if (last_used_block > BLOCK_CNT)
        return -2; // No space available for the operation

    // Update the bitmap with the blocks that will be occupied
    lb_bitmap <<= last_used_block;
    free_bitmap ^= lb_bitmap;

    // Create the file metadata
    memset(&dir_entry, 0x00, sizeof(flash_memory_dir_st));
    memcpy(dir_entry.filename, filename, FILENAME_SIZE);
    dir_entry.block_in = block_id;
    dir_entry.file_size = data_len;
    dir_entry.datetime_created = time(NULL);

    // Copy metadata and data to the block
    page_entry_ptr = disk_access.blocks[block_id].page_ptr[0];
    memcpy(page_entry_ptr, &dir_entry, sizeof(dir_entry));
    memcpy(page_entry_ptr + sizeof (dir_entry), data, data_len);

    return 0; // Success
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
