#include <ctype.h>
#include <server/irc.h>

/*
** the only restriction on a
** channel name is that it may not contain any spaces (' '), a control G
** (^G or ASCII 7), or a comma (',' which is used as a list item
** separator by the protocol)
*/

static int irc_part_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    if (!tokens[1].addr || tokens[2].addr)
    {
        irc_reply(e, cs, ERR_NEEDMOREPARAMS, NULL);
        return (-1);
    }

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    if (strpbrk(channel, "\x07\x2C"))
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else if (channel_len - 1 > CHANNELSTRSIZE)
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else if ((channel[0] != '#' && channel[0] != '&') ||
             !is_valid_chan_name(channel))
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else if (channel_len < 1)
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else
        return (0);
    return (-1);
}

int irc_part(t_env *e, int cs, t_token *tokens)
{
    char   concat[CHANNELSTRSIZE + NICKNAMESTRSIZE + 11];
    size_t i;

    if ((irc_part_check_command(e, cs, tokens)) != 0)
        return (-1);

    // Look for already existing channel or create it
    i = 0;
    while (i < e->maxchannels)
    {
        if (strncmp(e->channels[i].channel, tokens[1].addr, tokens[1].len) == 0)
            break;
        i++;
    }

    if (i == e->maxchannels)
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, tokens[1].addr);
        return (-1);
    }

    if (e->fds[cs].channel != i)
    {
        irc_reply(e, cs, ERR_NOTONCHANNEL, tokens[1].addr);
        return (-1);
    }

    memset(concat, 0, sizeof(concat));

    sprintf(concat, "%s leaved %s.\n", e->fds[cs].nickname,
            e->channels[e->fds[cs].channel].channel);

    broadcast(e, concat, IRC_NOTICE, cs);

    loginfo(" %s leaved %s\n", e->fds[cs].nickname,
            e->channels[e->fds[cs].channel].channel);

    e->channels[e->fds[cs].channel].clients--;

    // Clear the channel
    if (e->fds[cs].channel != 0 && e->channels[e->fds[cs].channel].clients == 0)
        memset(&e->channels[e->fds[cs].channel], 0, sizeof(t_channel));

    e->fds[cs].channel = 0;
    e->channels[e->fds[cs].channel].clients++;

    return (IRC_JOIN);
}
