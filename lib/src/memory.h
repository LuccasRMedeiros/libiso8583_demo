#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#define MEMORY_SIZE 4096
#define BLOCK_SIZE  512
#define PAGE_SIZE   256
#define BLOCK_CNT   MEMORY_SIZE/BLOCK_SIZE
#define PAGE_CNT    BLOCK_SIZE/PAGE_SIZE
#define FILENAME_SIZE   64

typedef unsigned char byte;

typedef struct
{
    uint16_t block_size;
    uint8_t total_blocks;
    uint8_t free_bitmap;
} __attribute__((packed)) flash_memory_filesystem_st;

typedef struct
{
    char filename[FILENAME_SIZE];
    uint8_t block_in;
    uint32_t file_size;
    time_t datetime_created;
} __attribute__((packed)) flash_memory_dir_st;

typedef struct 
{
    byte *entry_ptr;

    struct
    {
        byte *page_ptr[PAGE_CNT];
    } blocks[BLOCK_CNT];
} flash_memory_map_st;

void memory_init(void);
int memory_erase_block(size_t block_id);
int flash_memory_write(char *filename, void *data, size_t data_len);
int flash_memory_read(flash_memory_dir_st **out);

#endif
