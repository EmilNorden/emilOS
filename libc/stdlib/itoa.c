#include <stdlib.h>
#include <string.h>

static char itoa_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static void putc(char* buf, int len, int *index, char c) {
    if(*index >= len)
        return;
    buf[(*index)++] = c;
}

char* itoa(int val, char* buf, int len, int base) {
    int index = 0;

    if(val == 0) {
        putc(buf, len, &index, '0');
        putc(buf, len, &index, '\0');
        return buf;
    }

    if(val < 0 && base == 10) {
        putc(buf, len, &index, '-');
        val = abs(val);
    }

    while(val > 0) {
        putc(buf, len, &index, itoa_chars[val % base]);
        val = val / base;
    }

    putc(buf, len, &index, '\0');

    strrev(buf, index);

    return buf;
}

char* itoa64(long long val, char* buf, int len, int base) {
    int index = 0;

    if(val == 0) {
        putc(buf, len, &index, '0');
        putc(buf, len, &index, '\0');
        return buf;
    }

    if(val < 0 && base == 10) {
        putc(buf, len, &index, '-');
        val = abs(val);
    }

    while(val > 0) {
        putc(buf, len, &index, itoa_chars[val % base]);
        val = val / base;
    }

    putc(buf, len, &index, '\0');

    strrev(buf, index);

    return buf;
}