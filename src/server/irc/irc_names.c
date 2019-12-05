#include <server/irc.h>

/*
** Check if buffer is full for new name
*/

static int is_names_buf_full(const char *buf, size_t buf_size, const char *name)
{
    return ((buf_size - strlen(buf)) < (strlen(name) + 1));
}

static int irc_names_all_channels(t_env *e, int cs, t_token *tokens)
{
    char   concat[512];
    size_t i;
    size_t j;

    (void)tokens;

    i = 0;
    while (i < e->maxchannels)
    {
        if (e->channels[i].channel[0])
        {
            memset(concat, 0, sizeof(concat));

            j = 0;
            while (j <= e->max)
            {
                if (is_names_buf_full(concat, sizeof(concat),
                                      e->fds[j].nickname))
                    break;

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

static int irc_names_one_channels(t_env *e, int cs, t_token *tokens)
{
    char   concat[512];
    size_t i;
    size_t j;
    size_t k;

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
                    if (is_names_buf_full(concat, sizeof(concat),
                                          e->fds[j].nickname))
                        break;

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

int irc_names(t_env *e, int cs, t_token *tokens)
{
    // When no channel provided
    if (!tokens[1].addr)
        return (irc_names_all_channels(e, cs, tokens));

    return (irc_names_one_channels(e, cs, tokens));
}
