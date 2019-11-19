#include <client/irc.h>

size_t tokenize(char *str, t_token *tokens, size_t len)
{
    size_t i;
    size_t count;

    i = 0;
    count = 0;
    while (str[i] && str[i] != 0x0A && str[i] != 0x0D && count < len)
    {
        // Skip whitespaces
        while (str[i] == 0x20 && str[i])
            i++;

        if (str[i] == 0 || str[i] == 0x0A)
            break;

        tokens[count].addr = str + i;

        // Skip printable
        while (str[i] != 0x20 && str[i] != 0x0A && str[i] != 0x0D && str[i])
        {
            tokens[count].len++;
            i++;
        }

        count++;
    }

    return (count);
}
