#include <time.h>
#include <irc.h>

int time2iso(char* str)
{
    time_t     timer;
    struct tm *tm_info;

    time(&timer);
    tm_info = localtime(&timer);
    if (strftime(str, ISOTIMESTRSIZE, "%Y-%m-%d %H:%M:%S%z", tm_info) == 0)
        return (-1);
    return (0);
}