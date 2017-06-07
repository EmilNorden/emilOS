#include <stdlib.h>

int abs(int val)
{
    if(val < 0)
        return -val;
    return val;
}

long long abs64(long long val)
{
    if(val < 0)
        return -val;
    return val;
}