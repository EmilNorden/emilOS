#include "kernel/page_frame_allocator.h"
#include "kernel/multiboot.h"
#include "kernel/pool_allocator.h"
#include <stdint.h>
#include <string.h>

#define PAGE_SIZE           (2*1024*1024)
#define PAGES_PER_BYTE      (8)

static uint8_t* palloc_page_bitmap;
static uint32_t palloc_page_count;

static uint64_t palloc_get_total_memory(multiboot_info_t* mbi)
{
    multiboot_memory_map_t *mmap;
    uint64_t result = 0;

    mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
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
    uint64_t total_memory = palloc_get_total_memory(mbi);
    
    size_t no_pages = total_memory / PAGE_SIZE;
    size_t bitmap_size = no_pages / PAGES_PER_BYTE;

    palloc_page_count = no_pages;
    palloc_page_bitmap = (uint8_t*)pool_alloc(bitmap_size);
    memset(palloc_page_bitmap, 0, bitmap_size);
}