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
        return (irc_err(e, cs, ERR_NEEDMOREPARAMS, NULL));

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    if (channel_len - 1 > CHANNELSTRSIZE)
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
    else if (!is_valid_chan_name(channel, channel_len))
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
    else if (channel_len < 1)
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
    return (0);
}

int irc_part(t_env *e, int cs, t_token *tokens)
{
    char   concat[CHANNELSTRSIZE + NICKNAMESTRSIZE + 11];
    size_t i;

    logdebug("irc_part:: %s", tokens[0].addr);
    logdebug("irc_part:: leaving %s", tokens[1].addr);

    if ((irc_part_check_command(e, cs, tokens)) < 0)
        return (-1);

    // Look for already existing channel or create it
    i = 0;
    while (i < e->maxchannels)
    {
        if (strncmp(e->channels[i].channel, tokens[1].addr, tokens[1].len) ==
                0 &&
            e->channels[i].channel[tokens[1].len] == 0)
            break;
        i++;
    }

    if (i == e->maxchannels)
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, tokens[1].addr));

    if (e->fds[cs].channel != i)
        return (irc_err(e, cs, ERR_NOTONCHANNEL, tokens[1].addr));

    if (e->fds[cs].channel == 0)
        return (IRC_PART);

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

    irc_user_join_default_channel(e, cs);

    return (IRC_PART);
}
