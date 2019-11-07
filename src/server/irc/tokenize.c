#include "server/irc.h"

size_t tokenize(char *str, char **tokens, size_t len)
{
    size_t i;
    size_t count;

    i = 0;
    count = 0;
    while (str[i] && count < len)
    {
        // Skip whitespaces
        while ((str[i] == 0x0A || str[i] == 0x09) && str[i])
            i++;

        if (!str[i])
            break;

        memcpy(&tokens[count], &str[i], sizeof(char*));
        count++;
        // tokens[count++] = &str[i];

        // Skip printable
        while (str[i] != 0x0A && str[i] != 0x09 && str[i])
            i++;
    }

    return i;
}