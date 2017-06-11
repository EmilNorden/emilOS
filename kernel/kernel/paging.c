#include "kernel/paging.h"
#include "kernel/page_frame_allocator.h"
#include <stdio.h>
#include <stdint.h>

extern void set_page_directory(uint32_t*);
extern void enable_paging(void);

void paging_init(void)
{
    printf("Init paging...\n");
    void* page = palloc();
    uint32_t* page_directory = (uint32_t*)page;
    uint32_t* page_table = page_directory+1024;

    for(int i = 0; i < 1024; ++i) {
        page_directory[i] = 0x00000002;
    }

    for(int i = 0; i < 1024; ++i) {
        page_table[i] = (i * 0x1000) | 3;
    }

    page_directory[0] = (uint32_t)page_table | 3;

    set_page_directory(page_directory);
    enable_paging();
    printf("Paging enabled.\n");
}
