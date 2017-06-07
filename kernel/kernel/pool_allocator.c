#include "kernel/pool_allocator.h"
#include <stdint.h>

#define MEMORY_ALIGNMENT   8

extern void* __HEAP_START;
static void* pool_current;

void pool_init(void)
{
    pool_current = &__HEAP_START;
}

void* pool_alloc(size_t size)
{
    size_t rem = size % MEMORY_ALIGNMENT;
    if(rem > 0) {
        size += MEMORY_ALIGNMENT - rem;
    }

    void* ptr = pool_current;
    pool_current = ((uint8_t*)pool_current) + size;
    return ptr;
}