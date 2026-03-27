#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#define FILENAME_SIZE   64

typedef unsigned char byte;

typedef struct
{
    byte filename[FILENAME_SIZE];
    uint8_t block_in;
    uint32_t file_size;
    time_t datetime_created;
} __attribute__((packed)) memory_dir_st;

void memory_init(void);
int memory_erase_block(size_t block_id);
int memory_write(char *filename, void *data, size_t data_len);
int memory_get_str_filename(byte *dir_filename, char *out);
int memory_read(memory_dir_st **out);
int memory_read_file(const char *filename, byte **out);
void memory_show_info(void);

#endif
