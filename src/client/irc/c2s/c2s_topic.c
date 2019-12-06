#include <client/irc.h>

static int c2s_topic_check_command(t_env *e, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    if (!tokens[1].addr)
        return (irc_error(e, ERR_NEEDMOREPARAMS, tokens[0].addr));

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    if (channel_len - 1 > CHANNELSTRSIZE)
        return (irc_error(e, ERR_NOSUCHCHANNEL, tokens[1].addr));
    else if (!is_valid_chan_name(channel))
        return (irc_error(e, ERR_NOSUCHCHANNEL, tokens[1].addr));
    else if (channel_len < 1)
        return (irc_error(e, ERR_NOSUCHCHANNEL, ""));
    return (0);
}

int _c2s_topic(t_fd *fd, const char *chan, size_t chan_len, const char *msg)
{
    if (msg)
        return (cbuffer_putcmd(&fd->buf_write, "TOPIC %.*s :%s\x0D\x0A", chan_len, chan, msg));
    return (cbuffer_putcmd(&fd->buf_write, "TOPIC %.*s\x0D\x0A", chan_len, chan));
}

int c2s_topic(t_env *e, t_token *tokens)
{
    if (e->sock == -1)
        return logerror(
            "You need to be logged in before any command. Use "
            "/connect [server] ?[port]");

    if (c2s_topic_check_command(e, tokens) < 0)
        return (-1);

    _c2s_topic(e->self, tokens[1].addr, tokens[1].len, tokens[2].addr);

    return (IRC_TOPIC);
}
