#include <irc.h>

size_t strlentrim(const char *str)
{
    size_t i;
    size_t j;

    i = 0;
    while (str[i] == ' ' || str[i] == '\n')
        i++;

    if (str[0] == 0)
        j = 0;
    else
        j = strlen(str) - 1;
    while (j && str[j] != ' ' && str[j] != '\n')
        j--;
    return (j - i);
}

const char *strtrim(const char *str)
{
    size_t i;

    i = 0;
    while (str[i] == ' ' || str[i] == '\n')
        i++;
    return (&str[i]);
}