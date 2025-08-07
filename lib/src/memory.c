#include "memory.h"

#include <string.h>

static byte memory_disk[MEMORY_SIZE];
static memory_disk_accessors_st disk_access;


void memory_init(void)
{
    byte *memory_disk_ptr = memory_disk;

    memset(memory_disk, 0xff, sizeof (memory_disk));
    disk_access.entry_ptr = memory_disk;
    disk_access.block_size = BLOCK_SIZE;
    disk_access.total_blocks = BLOCK_CNT;
    disk_access.free_bitmap = 0b01111111; // All blocks are suposed to available except block 0 (reserved for filesystem)

    for (size_t block_id = 0; block_id < BLOCK_CNT; ++block_id)
    {
        for(size_t page_id = 0; page_id < PAGE_CNT; ++page_id)
        {
            disk_access.block[block_id].page_ptr[page_id] = memory_disk_ptr;
            memory_disk_ptr += PAGE_SIZE; // Equivalent to ((block id * total blocks) + page id) + page size
        }
    }

    // Writes the filesystem on memory
    memcpy(disk_access.block[0].page_ptr, disk_access.block_size, sizeof (disk_access.block_size));
    memcpy(disk_access.block[0].page_ptr + sizeof(disk_access.block_size), disk_access.total_blocks, sizeof (disk_access.total_blocks));
    memcpy(disk_access.block[0].page_ptr + sizeof(disk_access.block_size) + sizeof (disk_access.total_blocks), disk_access.free_bitmap, sizeof (disk_access.free_bitmap));
}

int memory_erase_block(size_t block_id)
{
    if (block_id <= 0 || block_id >= BLOCK_CNT)
        return -1; // Out of bounds (or want to erase filesystem)

    memset(disk_access.block[block_id].page_ptr[0], 0xff, BLOCK_SIZE);

    return 0;
}

