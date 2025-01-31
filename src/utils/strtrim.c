#include <irc.h>

size_t strlentrim(const char *str)
{
    size_t i;
    size_t j;

    i = 0;
    while (str[i] == ' ' || str[i] == '\n')
        i++;

    if (str[i] == 0)
        return (0);

    j = strlen(str) - 1;
    while (j && str[j] == ' ' && str[j] == '\n')
    {
        j--;
        i++;
    }
    return (strlen(str) - i);
}

const char *strtrim(const char *str)
{
    size_t i;

    i = 0;
    while (str[i] == ' ' || str[i] == '\n')
        i++;
    return (&str[i]);
}