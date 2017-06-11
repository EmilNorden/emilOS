#include "kernel/page_frame_allocator.h"
#include "kernel/multiboot.h"
#include "kernel/pool_allocator.h"
#include "kernel/debug.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PAGE_SIZE           (2*1024*1024)
#define PAGES_PER_BYTE      (8)
#define PAGES_PER_INT       (32)
#define IS_BIT_SET(X, Y)    (X & (1 << Y)) 

static void process_memory_map(multiboot_info_t*);
static void reserve_heap_pages(void);
static inline bool is_bit_set(uint32_t value, uint32_t bit);
static inline uint8_t disable_bit8(uint8_t value, uint32_t bit);
static inline uint8_t enable_bit8(uint8_t value, uint32_t bit);
static inline uint32_t disable_bit32(uint32_t value, uint32_t bit);
static inline void deallocate_page(uint32_t page);
static inline void allocate_page(uint32_t page);

/* Array storing free pages. 1bit per page. 1 is free, 0 is not. */
static uint8_t* palloc_page_bitmap;
static uint32_t palloc_page_count;

static uint64_t palloc_get_mmap_size(multiboot_info_t* mbi)
{
    uint64_t result = 0;

    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
    while((unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length) {
        
        uint64_t region_end = mmap->addr + mmap->len;
        if(region_end > result)
            result = region_end;
        
        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size));
    }

    return result;
}

void palloc_init(multiboot_info_t* mbi)
{
    printf("Init page frame allocator...\n");
    uint64_t mmap_size = palloc_get_mmap_size(mbi);
    
    size_t page_count = mmap_size / PAGE_SIZE;
    size_t bitmap_size = page_count / PAGES_PER_BYTE;

    palloc_page_count = page_count;
    palloc_page_bitmap = (uint8_t*)pool_alloc(bitmap_size);
    memset(palloc_page_bitmap, 0xFF, bitmap_size);
    process_memory_map(mbi);
    reserve_heap_pages();
}

static void process_memory_map(multiboot_info_t* mbi)
{
    uint64_t total_physmem = 0;
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
    while((unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length) {

        if(mmap->type == 1) {
            total_physmem += mmap->len;
        }
        else {
            uint64_t start_page = mmap->addr / PAGE_SIZE;
            uint64_t end_page = (mmap->addr + mmap->len-1) / PAGE_SIZE;
        
            for(uint32_t page = start_page; page < end_page; ++page) {
                allocate_page(page);
            }
        }

        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size));
    }

    printf("Found %l MB of RAM.\n", total_physmem / (1024*1024));
}

static void reserve_heap_pages(void)
{
    void* heap_begin = pool_heap_begin();
    void* heap_end = pool_heap_end();
    uint32_t heap_start_page = (uint32_t)heap_begin / PAGE_SIZE;
    uint32_t heap_end_page = (uint32_t)heap_end / PAGE_SIZE;
    printf("Reserving heap memory at 0x%p to 0x%p\n", pool_heap_begin(), pool_heap_end());
    for(uint32_t page = heap_start_page; page < heap_end_page; ++page) {
        allocate_page(page);
    }
}

void* palloc(void)
{
    uint32_t* bmp = (uint32_t*)palloc_page_bitmap;

    for(uint32_t i = 0; i < palloc_page_count / PAGES_PER_INT; ++i) {
        if(bmp[i] == 0) 
            continue;

        for(int bit = 0; bit < 32; ++bit) {
            if(!is_bit_set(bmp[i], bit)) 
                continue;

            bmp[i] = disable_bit32(bmp[i], bit);
            uint32_t page_index = (i * PAGES_PER_INT) + bit;
            return (void*)(page_index * PAGE_SIZE);
        }
    }

    return NULL;
}

void pfree(void* pageptr)
{
    //Assert that page pointer is aligned to page boundary? Otherwise it may be a stray pointer.
    uint32_t page = ((uint32_t)pageptr) / PAGE_SIZE;
    deallocate_page(page);
}

static inline bool is_bit_set(uint32_t value, uint32_t bit)
{
    return value & (1 << bit);
}

static inline uint8_t disable_bit8(uint8_t value, uint32_t bit)
{
    return value & ~(1 << bit);
}

static inline uint8_t enable_bit8(uint8_t value, uint32_t bit)
{
    return value | (1 << bit);
}

static inline uint32_t disable_bit32(uint32_t value, uint32_t bit)
{
    return value & ~(1 << bit);
}

static inline void allocate_page(uint32_t page)
{
    uint32_t index = page / PAGES_PER_BYTE;
    uint32_t bit_index = page % PAGES_PER_BYTE;

    palloc_page_bitmap[index] = disable_bit8(palloc_page_bitmap[index], bit_index);
}

static inline void deallocate_page(uint32_t page)
{
    uint32_t index = page / PAGES_PER_BYTE;
    uint32_t bit_index = page % PAGES_PER_BYTE;

    palloc_page_bitmap[index] = enable_bit8(palloc_page_bitmap[index], bit_index);
}