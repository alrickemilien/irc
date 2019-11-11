#include <stdio.h>

#include <irc.h>

#define LOG_FORMAT "[%s]: %s\n"
#define LOGSIZE 512

int loginfo(const char *fmt, ...)
{
    va_list ap;
    char final_fmt[LOGSIZE + 1];
    char t[ISOTIMESTRSIZE];

    time2iso(t);
    sprintf(final_fmt, LOG_FORMAT, t, fmt);
    va_start(ap, fmt);
    vprintf(final_fmt, ap);
    va_end(ap);

    return (0);
}