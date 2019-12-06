#include <server/irc.h>

static int irc_topic_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    if (!tokens[1].addr)
        return (irc_err(e, cs, ERR_NEEDMOREPARAMS, NULL));

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    // When invalid channel
    if (channel_len - 1 > CHANNELSTRSIZE)
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
    else if (!is_valid_chan_name(channel, channel_len))
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
    else if (channel_len < 1)
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));

    // When cs is not into channel
    else if (strncmp(e->channels[e->fds[cs].channel].channel, channel,
                     channel_len) != 0 ||
             e->channels[e->fds[cs].channel].channel[channel_len] != 0)
        return (irc_err(e, cs, ERR_NOTONCHANNEL, channel));

    else if (e->channels[e->fds[cs].channel].chop != cs)
        return (irc_err(e, cs, ERR_CHANOPRIVSNEEDED, channel));

    return (0);
}

int irc_topic(t_env *e, int cs, t_token *tokens)
{
    if (irc_topic_check_command(e, cs, tokens) < 0)
        return (-1);

    if (e->channels[e->fds[cs].channel].topic[0] == 0 && !tokens[2].addr)
    {
        irc_reply(e, cs, RPL_NOTOPIC, e->channels[e->fds[cs].channel].channel);
        return (IRC_TOPIC);
    }

    if (tokens[2].addr)
        memrpl(e->channels[e->fds[cs].channel].topic, TOPICSTRSIZE + 1,
               tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr,
               tokens[2].addr[0] == ':' ? tokens[2].len - 1 : tokens[2].len);

    irc_reply_all_in_channel(e, cs, RPL_TOPIC,
                             e->channels[e->fds[cs].channel].channel,
                             e->channels[e->fds[cs].channel].topic);

    return (IRC_TOPIC);
}
