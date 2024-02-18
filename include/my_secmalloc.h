#ifndef MY_SECMALLOC_H
#define MY_SECMALLOC_H

#include <stddef.h>

typedef struct {
    void* start;
    size_t size;
    int is_free;
    int blocks;
    size_t allocation_size;
    unsigned long canary_start;
    unsigned long canary_end;
} BlockMeta;

#define POOL_SIZE 1024 * 1024
#define MAX_BLOCKS 128

extern BlockMeta* meta_pool;
extern void* data_pool;

void init_memory_pool();
void* my_malloc(size_t requested_size);
void my_free(void* ptr);

#endif // MY_SECMALLOC_H
