#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>

extern "C" {

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
	terminal_initialize();

	if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("This OS must be loaded by a Multiboot-compliant boot loader!\n");
		return;
	}

	printf("Hello, world!");
}
}