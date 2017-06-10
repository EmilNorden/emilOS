#ifndef _PAGE_FRAME_ALLOCATOR_H_
#define _PAGE_FRAME_ALLOCATOR_H_

struct multiboot_info;

void palloc_init(struct multiboot_info*);
void* palloc(void);
void pfree(void*);

#endif