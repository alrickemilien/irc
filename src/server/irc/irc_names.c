#include <ctype.h>
#include "server/irc.h"

int irc_names(t_env *e, int cs, t_token *tokens)
{
    // char   concat[512];
    size_t i;
    size_t j;

    j = 1;
    while (tokens[j].addr)
    {
        // "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
        strncat(e->fds[cs].buf_write, tokens[j].addr, tokens[j].len);
        strcat(e->fds[cs].buf_write, " :");

        i = 0;
        while (i <= e->max)
        {
            if (e->fds[i].type == FD_CLIENT &&
                strncmp(e->fds[i].channel, tokens[j].addr, tokens[j].len) == 0)
            {
                strcat(e->fds[cs].buf_write, " ");
                strcat(e->fds[cs].buf_write, e->fds[i].nickname);
            }
            i++;
        }

        j++;
    }

    return (IRC_NAMES);
}
