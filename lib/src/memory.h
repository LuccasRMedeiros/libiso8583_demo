#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define MEMORY_SIZE 4096
#define BLOCK_SIZE  512
#define PAGE_SIZE   256
#define BLOCK_CNT   MEMORY_SIZE/BLOCK_SIZE
#define PAGE_CNT    BLOCK_SIZE/PAGE_SIZE

typedef unsigned char byte;

typedef struct 
{
    byte *entry_ptr;

    uint16_t block_size;
    uint8_t total_blocks;
    uint8_t free_bitmap;

    struct
    {
        byte *page_ptr[PAGE_CNT];
    } block[BLOCK_CNT];
} flash_memory_access_st;

void memory_init(void);
int memory_erase_block(size_t block_id);

#endif
