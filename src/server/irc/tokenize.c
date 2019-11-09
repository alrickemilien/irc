#include "server/irc.h"

size_t tokenize(char *str, t_token *tokens, size_t len)
{
    size_t i;
    size_t count;
    char * end;

    end = strstr(str, "\x0D\x0A");
    i = 0;
    count = 0;
    while (str[i] && str + i < end && count < len)
    {
        // Skip whitespaces
        while (str[i] == 0x20 && str[i])
            i++;

        if (str[i] == 0)
            break;

        tokens[count].addr = str + i;

        // Skip printable
        while (str[i] != 0x20 && str[i])
            i++;
        tokens[count].len = (size_t)(&str[i] - tokens[count].addr);

        count++;
    }

    return (i);
}