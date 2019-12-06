#include <client/irc.h>
#include <ctype.h>

/*
** the only restriction on a
** channel name is that it may not contain any spaces (' '), a control G
** (^G or ASCII 7), or a comma (',' which is used as a list item
** separator by the protocol)
*/

static int c2s_join_check_command(t_env *e, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    if (!tokens[1].addr || tokens[2].addr)
        return (irc_error(e, ERR_NEEDMOREPARAMS, tokens[0].addr));

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    if (strpbrk(channel, "\x07\x2C"))
        return (irc_error(e, ERR_NOSUCHCHANNEL, tokens[1].addr));
    else if (channel_len - 1 > CHANNELSTRSIZE)
        return (irc_error(e, ERR_NOSUCHCHANNEL, tokens[1].addr));
    else if (!is_valid_chan_name(channel, channel_len))
        return (irc_error(e, ERR_NOSUCHCHANNEL, tokens[1].addr));
    else if (channel_len < 1)
        return (irc_error(e, ERR_NOSUCHCHANNEL, ""));
    else
        return (0);
    return (-1);
}

int _c2s_join(t_fd *fd, const char *channel_name, size_t channel_name_len)
{
    logdebug("JOIN %.*s", channel_name_len, channel_name);
    return (cbuffer_putcmd(&fd->buf_write, "JOIN %.*s\x0D\x0A",
                           channel_name_len, channel_name));
}

int c2s_join(t_env *e, t_token *tokens)
{
    if (e->sock == -1)
        return (irc_error(e, ERR_NOT_CONNECTED));

    if ((c2s_join_check_command(e, tokens)) != 0)
        return (-1);

    _c2s_join(e->self, tokens[1].addr, tokens[1].len);

    return (IRC_C2S_JOIN);
}
