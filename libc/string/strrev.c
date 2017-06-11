void strrev(char *buf, int len)
{
    int lo = 0;
    int hi = len-2;
    while(lo < hi) {
        char tmp = buf[lo];
        buf[lo++] = buf[hi];
        buf[hi--] = tmp;
    }
}