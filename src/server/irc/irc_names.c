#include <ctype.h>
#include "server/irc.h"

int irc_names(t_env *e, int cs, t_token *tokens)
{
    size_t i;
    size_t j;
    size_t k;
    char   concat[512];

    // When no channel provided
    if (!tokens[1].addr)
    {
        i = 0;
        while (i < e->maxchannels && e->channels[i].channel[0])
        {
            memset(concat, 0, sizeof(concat));

            j = 0;
            while (j <= e->max)
            {
                if (e->fds[j].type == FD_CLIENT && e->fds[j].channel == i)
                {
                    strcat(concat, e->fds[j].nickname);
                    strcat(concat, " ");
                }
            }
            i++;

            irc_reply(e, cs, RPL_NAMREPLY, e->channels[i].channel, concat);
            irc_reply(e, cs, RPL_ENDOFNAMES, NULL);
        }

        return (IRC_NAMES);
    }

    i = 0;
    while (i < e->maxchannels && e->channels[i].channel[0])
    {
        k = 1;
        while (tokens[k].addr)
        {
            if (strncmp(e->channels[i].channel, tokens[k].addr,
                        tokens[k].len) == 0)
            {
                // "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
                memset(concat, 0, sizeof(concat));

                j = 0;
                while (j <= e->max)
                {
                    if (e->fds[j].type == FD_CLIENT && e->fds[j].channel == i)
                    {
                        strcat(concat, e->fds[j].nickname);
                        strcat(concat, " ");
                    }
                }

                irc_reply(e, cs, RPL_NAMREPLY, e->channels[i].channel, concat);
                irc_reply(e, cs, RPL_ENDOFNAMES, NULL);

                break;
            }
            k++;
        }

        i++;
    }

    // When concat is still zrero meaning that no channel found
    if (concat[0] == 0)
        irc_reply(e, cs, RPL_ENDOFNAMES, NULL);

    return (IRC_NAMES);
}
