#include <server/irc.h>

int irc_list(t_env *e, int cs, t_token *tokens)
{
    size_t  i;
    size_t  j;
    size_t  subtoken_count;
    t_token subtokens[30];

    irc_reply(e, cs, RPL_LISTSTART, NULL);

    // When no channel provided
    if (!tokens[1].addr)
    {
        i = 0;
        while (i < e->maxchannels)
        {
            if (i == 0 ||
                (e->channels[i].channel[0] && e->channels[i].clients > 0))
                irc_reply(e, cs, RPL_LIST, e->channels[i].channel, e->channels[i].topic);
            i++;
        }
        irc_reply(e, cs, RPL_LISTEND, NULL);
        return (IRC_LIST);
    }

    subtoken_count = tokenizechr(tokens[1].addr, subtokens, 30, ',');

    i = 0;
    while (i < e->maxchannels)
    {
        j = 0;
        while (j < subtoken_count)
        {
            if (strncmp(e->channels[e->fds[i].channel].channel,
                        subtokens[j].addr, subtokens[j].len) == 0)
                irc_reply(e, cs, RPL_LIST, e->channels[i].channel,
                          e->channels[i].topic);
            j++;
        }

        irc_reply(e, cs, RPL_LIST, e->channels[i].channel,
                  e->channels[i].topic);
        i++;
    }

    // When concat is still zrero meaning that no channel found
    irc_reply(e, cs, RPL_LISTEND, NULL);

    return (IRC_LIST);
}
