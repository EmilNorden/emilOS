#include <stdlib.h>

static void putc(char* buf, int len, int *index, char c) {
    if(*index >= len)
        return;
    buf[(*index)++] = c;
}

static void strrev(char *buf, int len) {
    int lo = 0;
    int hi = len-2;
    while(lo < hi) {
        char tmp = buf[lo];
        buf[lo++] = buf[hi];
        buf[hi--] = tmp;
    }
}

char* itoa(int val, char* buf, int len, int base) {
    int index = 0;

    if(val == 0) {
        putc(buf, len, &index, '0');
        putc(buf, len, &index, '\0');
        return buf;
    }

    if(val < 0) {
        putc(buf, len, &index, '-');
        val = abs(val);
    }

    while(val > 0) {
        putc(buf, len, &index, '0' + (val % base));
        val = val / base;
    }

    putc(buf, len, &index, '\0');

    strrev(buf, index);

    return buf;
}