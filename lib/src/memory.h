#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

#define MEMORY_SIZE 4096
#define BLOCK_SIZE  4096
#define PAGE_SIZE   512
#define BLOCK_CNT   MEMORY_SIZE/BLOCK_SIZE
#define PAGE_CNT    BLOCK_SIZE/PAGE_SIZE

typedef unsigned char byte;

typedef struct 
{
    byte *entry_ptr;

    struct
    {
        byte *page_ptr[PAGE_CNT];
    } block[BLOCK_CNT];
} memory_disk_accessors;

void memory_init(void);
int memory_erase_block(size_t block_id);

#endif
