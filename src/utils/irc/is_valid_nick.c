#include <ctype.h>
#include <irc.h>

/*
** <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
** <number>     ::= '0' ... '9'
** <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
*/

static const char *g_valid_nick_char = "-[]\x5C\x60^{}";

bool is_valid_nick(const char *nick)
{
    size_t i;

    i = 0;
    while (nick[i] && nick[i] != '\x0D')
    {
        if (!isalnum(nick[i]) && !strchr(g_valid_nick_char, nick[i]))
            return (false);
        i++;
    }
    return (true);
}