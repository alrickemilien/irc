#include <ctype.h>
#include <server/irc.h>

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
        while (i < e->maxchannels)
        {
            if (e->channels[i].channel[0])
            {
                memset(concat, 0, sizeof(concat));

                j = 0;
                while (j <= e->max)
                {
                    if (e->fds[j].type == FD_CLIENT && e->fds[j].registered &&
                        e->fds[j].channel == i)
                    {
                        strcat(concat, e->fds[j].nickname);
                        strcat(concat, " ");
                    }
                    j++;
                }

                irc_reply(e, cs, RPL_NAMREPLY, e->channels[i].channel, concat);
                irc_reply(e, cs, RPL_ENDOFNAMES, e->channels[i].channel);
            }

            i++;
        }

        return (IRC_NAMES);
    }

    i = 0;
    while (i < e->maxchannels)
    {
        k = 1;
        while (e->channels[i].channel[0] && tokens[k].addr)
        {
            if (strncmp(e->channels[i].channel, tokens[k].addr,
                        tokens[k].len) == 0)
            {
                // "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
                memset(concat, 0, sizeof(concat));

                j = 0;
                while (j <= e->max)
                {
                    if (e->fds[j].type == FD_CLIENT && e->fds[j].channel == i &&
                        e->fds[j].registered == 1)
                    {
                        strcat(concat, e->fds[j].nickname);
                        strcat(concat, " ");
                    }
                    j++;
                }

                irc_reply(e, cs, RPL_NAMREPLY, e->channels[i].channel, concat);
                irc_reply(e, cs, RPL_ENDOFNAMES, e->channels[i].channel);

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
