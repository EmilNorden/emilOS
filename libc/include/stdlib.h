#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);
int abs(int);
long long abs64(long long);
char* itoa(int val, char* buf, int len, int base);
char* itoa64(long long val, char* buf, int len, int base);

#ifdef __cplusplus
}
#endif

#endif
