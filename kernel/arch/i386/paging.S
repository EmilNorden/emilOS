.section .text

.global enable_pae
.type enable_pae, @function
enable_pae:
push %ebp
mov %esp, %ebp
movl %cr4, %eax
bts $5, %eax
movl %eax, %cr4
mov %ebp, %esp
pop %ebp
ret


.global set_page_dir_ptr_table
.type set_page_dir_ptr_table, @function
set_page_dir_ptr_table:
push %ebp
mov %esp, %ebp
mov 8(%esp), %eax
movl %eax, %cr3
mov %ebp, %esp
pop %ebp
ret

.global enable_paging
.type enable_paging, @function
enable_paging:
push %ebp
mov %esp, %ebp
movl %cr0, %eax
orl $0x80000000, %eax
movl %eax, %cr0
mov %ebp, %esp
pop %ebp
ret