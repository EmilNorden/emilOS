#ifndef _KERNEL_DEBUG_H_
#define _KERNEL_DEBUG_H_

#define DebugBreak asm("xchgw %bx, %bx");

#endif