#include "kernel/paging.h"
#include "kernel/page_frame_allocator.h"
#include <stdio.h>
#include <stdint.h>

extern void enable_pae(void);
extern void set_page_dir_ptr_table(uint64_t*);
extern void enable_paging(void);

#define PRESENT     (1)
#define WRITABLE    (2)

void paging_init(void)
{
    printf("Init paging...\n");
    void* page = palloc();

    // Page directory pointer table should be aligned on a 32 byte boundary, which the page already is.
    uint64_t* page_dir_ptr_table = (uint64_t*)page;

    //  Page directory should be placed on a 4096 byte boundary, so put it 4096 bytes above page_dir_ptr_table
    uint64_t* page_directory = page_dir_ptr_table + 512;
    uint64_t* page_table = page_directory + 512;
    printf("page_dir_table: 0x%p\npage_directory: 0x%p\npage_table: 0x%p\n", page_dir_ptr_table, page_directory, page_table);

    page_dir_ptr_table[0] = (uint32_t)page_directory | PRESENT;
    page_directory[0] = (uint32_t)page_table | PRESENT | WRITABLE; 

    printf("page_dir_table[0] = %l\npage_directory[0] = %l\n", page_dir_ptr_table[0], page_directory[0]);

    // Map first 2 MB
    unsigned int address = 0;
    for(int i = 0; i < 512; ++i) {
        page_table[i] = address | PRESENT | WRITABLE;
        address = address + 0x1000;
    }

    // asm volatile ("movl %cr4, %eax; bts $5, %eax; movl %eax, %cr4"); // set bit5 in CR4 to enable PAE		 
    // asm volatile ("movl %%eax, %%cr3" :: "a" (page_dir_ptr_table)); // load PDPT into CR3
    // asm volatile ("movl %cr0, %eax; orl $0x80000000, %eax; movl %eax, %cr0;");
    enable_pae();
    set_page_dir_ptr_table(page_dir_ptr_table);
    enable_paging();

    printf("Paging enabled.\n");
}
