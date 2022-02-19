#include "log.h"


int log_printf_to_buffer(char *buff, int size, char *fmt, ...)
{
    int len = 0;
    va_list arg;
    va_start(arg, fmt);
    len += vsnprintf(buff, size, fmt, arg);
    va_end(arg);
    return len;
}
