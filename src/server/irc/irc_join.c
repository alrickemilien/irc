#include <ctype.h>
#include <server/irc.h>

/*
** the only restriction on a
** channel name is that it may not contain any spaces (' '), a control G
** (^G or ASCII 7), or a comma (',' which is used as a list item
** separator by the protocol)
*/

static bool is_valid_chan_name(const char *channel)
{
    size_t i;

    i = 1;
    while (channel[i] && channel[i] != '\x0D')
    {
        if (channel[i] == '\x07' || channel[i] == '\x20' ||
            channel[i] == '\x2C')
            return (false);
        i++;
    }
    return (true);
}

static int irc_join_check_command(t_env *e, int cs, const t_token *tokens)
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

int irc_join(t_env *e, int cs, t_token *tokens)
{
    char   concat[CHANNELSTRSIZE + NICKNAMESTRSIZE + 11];
    size_t i;
    size_t empty_channel;

    if ((irc_join_check_command(e, cs, tokens)) != 0)
        return (-1);

    // Look for already existing channel or create it
    i = 0;
    empty_channel = 0;
    while (i < e->maxchannels)
    {
        if (strncmp(e->channels[i].channel, tokens[1].addr, tokens[1].len) == 0)
            break;
        if (empty_channel == 0 && e->channels[i].channel[0] == 0)
            empty_channel = i;
        i++;
    }

    if (i == e->maxchannels)
        i = empty_channel;

    if (i == 0)
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, tokens[1].addr);
        return (-1);
    }

    memset(e->channels[i].channel, 0, CHANNELSTRSIZE + 1);
    strncpy(e->channels[i].channel, tokens[1].addr, tokens[1].len);

    e->channels[e->fds[cs].channel].clients--;

    // Clear the channel
    if (e->fds[cs].channel != 0 && e->channels[e->fds[cs].channel].clients == 0)
        memset(&e->channels[e->fds[cs].channel], 0, sizeof(t_channel));

    e->fds[cs].channel = i;

    memset(concat, 0, sizeof(concat));

    sprintf(concat, "%s!%s@%s JOIN %s\x0D\x0A", e->fds[cs].nickname,
            e->fds[cs].nickname, e->fds[cs].host,
            e->channels[e->fds[cs].channel].channel);

    broadcast_all_in_channel(e, concat, IRC_NOTICE, cs);

    loginfo("%s!%s@%s JOIN %s\n", e->fds[cs].nickname, e->fds[cs].nickname,
            e->fds[cs].host, e->channels[e->fds[cs].channel].channel);

    irc_reply(e, cs, RPL_TOPIC, e->channels[e->fds[cs].channel].channel,
              e->fds[cs].nickname);

    e->channels[i].clients++;

    return (IRC_JOIN);
}
