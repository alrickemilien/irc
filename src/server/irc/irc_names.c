#include <server/irc.h>

/*
** Check if buffer is full for new name
*/

static int is_names_buf_full(const char *buf, size_t buf_size, const char *name)
{
    return ((buf_size - strlen(buf)) < (strlen(name) + 1));
}

static int fill_names_buffer(t_env *e, int cs, size_t chan_index)
{
    char concat[512];
    size_t j;

    memset(concat, 0, sizeof(concat));
    j = 0;
    while (j <= e->max)
    {
        if (is_names_buf_full(concat, sizeof(concat), e->fds[j].nickname))
            break;

        if (e->fds[j].type == FD_CLIENT && e->fds[j].registered &&
            e->fds[j].channel == chan_index)
        {
            strcat(concat, e->fds[j].nickname);
            strcat(concat, " ");
        }

        j++;
    }

    irc_reply(e, cs, RPL_NAMREPLY, e->channels[chan_index].channel, concat);
    irc_reply(e, cs, RPL_ENDOFNAMES, e->channels[chan_index].channel);

    return (strlen(concat));
}

static int irc_names_all_channels(t_env *e, int cs, t_token *tokens)
{
    size_t i;

    (void)tokens;

    i = 0;
    while (i < e->maxchannels)
    {
        if (e->channels[i].channel[0])
            fill_names_buffer(e, cs, i);
        i++;
    }

    return (IRC_NAMES);
}

static int irc_names_one_channels(t_env *e, int cs, t_token *tokens)
{
    size_t i;
    size_t k;
    int ret;

    i = 0;
    ret = 0;
    while (i < e->maxchannels)
    {
        k = 1;
        while (e->channels[i].channel[0] && tokens[k].addr)
        {
            if (strncmp(e->channels[i].channel, tokens[k].addr,
                        tokens[k].len) == 0)
            {
                if (ret == 0)
                    ret = fill_names_buffer(e, cs, i);
                else
                    fill_names_buffer(e, cs, i);
                break;
            }
            k++;
        }
        i++;
    }

    // When concat is still zero meaning that no channel found
    if (ret == 0)
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
