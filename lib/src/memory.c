#include "memory.h"

#include <string.h>

static byte memory_disk[MEMORY_SIZE];
static memory_disk_accessors disk_access;

void memory_init(void)
{
    byte *memory_disk_ptr = memory_disk;

    memset(memory_disk, 0xff, sizeof (memory_disk));
    disk_access.entry_ptr = memory_disk;

    for (size_t block_id = 0; block_id < BLOCK_CNT; ++block_id)
    {
        for(size_t page_id = 0; page_id < PAGE_CNT; ++page_id)
        {
            disk_access.block[block_id].page_ptr[page_id] = memory_disk_ptr;
            memory_disk_ptr += PAGE_SIZE;
        }
    }
}

int memory_erase_block(size_t block_id)
{
    if (block_id >= BLOCK_CNT)
        return -1;

    memset(disk_access.block[block_id].page_ptr[0], 0xff, BLOCK_SIZE);

    return 0;
}

int memory_write(
