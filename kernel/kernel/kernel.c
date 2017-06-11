#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include "kernel/pool_allocator.h"
#include "kernel/page_frame_allocator.h"
#include "kernel/paging.h"

#define MEMORY_MAP_BITMASK (0b1000000)
#define HAS_MEMORY_MAP(x) (x & MULTIBOOT_INFO_MEM_MAP)

static void kernel_init(multiboot_info_t *mbi);

void kernel_main(multiboot_info_t* mbi, unsigned int magic)
{
	terminal_initialize();

	if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("This OS must be loaded by a Multiboot-compliant boot loader!\n");
		return;
	}

	//TODO: Move this into palloc_init?
	if(!HAS_MEMORY_MAP(mbi->flags)) {
		printf("No memory map provided by GRUB. The kernel cannot continue without it.");
		return;
	}

	kernel_init(mbi);

	printf("Hello, world!\n");
}

static void kernel_init(multiboot_info_t *mbi)
{
	pool_init();
	palloc_init(mbi);
	paging_init();
}