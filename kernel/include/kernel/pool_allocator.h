#ifndef _HEAP_POOL_ALLOCATOR_H_
#define _HEAP_POOL_ALLOCATOR_H_

#include <stddef.h>

void pool_init(void);
void* pool_alloc(size_t);
void pool_free(void*);
void* pool_heap_begin(void);
void* pool_heap_end(void);

#endif