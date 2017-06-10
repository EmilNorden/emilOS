#include "kernel/pool_allocator.h"
#include <stdint.h>
#include <stdio.h>

#define MEMORY_ALIGNMENT    (8)

extern void* __HEAP_START;
extern void* __HEAP_END;
static void* pool_current;

void pool_init(void)
{
    printf("Init heap pool allocator...\n");
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

    if(pool_current > pool_heap_end()) {
        // TODO: PANIC
    }

    return ptr;
}

void* pool_heap_begin(void)
{
    return &__HEAP_START;
}

void* pool_heap_end(void)
{
    return &__HEAP_END;
}