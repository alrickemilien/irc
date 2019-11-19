#include <time.h>
#include "irc.h"

void time2iso(char* str)
{
    time_t     timer;
    struct tm *tm_info;


    time(&timer);
    tm_info = localtime(&timer);
    strftime(str, ISOTIMESTRSIZE, "%Y-%m-%d %H:%M:%S%z", tm_info);
}