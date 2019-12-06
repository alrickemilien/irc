#include <client/irc.h>

static int c2s_invite_check_command(t_env *e, const t_token *tokens)
{
    if (!tokens[1].addr || !tokens[2].addr)
        return (irc_error(e, ERR_NEEDMOREPARAMS, tokens[0].addr));
    return (0);
}

int _c2s_invite(t_fd *      fd,
                const char *nick,
                size_t      nick_len,
                const char *channel)
{
    logdebug("INVITE %.*s :%s\x0D\x0A", nick_len, nick, channel);
    return (cbuffer_putcmd(&fd->buf_write, "INVITE %.*s %s\x0D\x0A", nick_len,
                           nick, channel));
}

int c2s_invite(t_env *e, t_token *tokens)
{
    if (e->sock == -1)
        return (irc_error(e, ERR_NOT_CONNECTED));

    if ((c2s_invite_check_command(e, tokens)) < 0)
        return (-1);

    _c2s_invite(e->self, tokens[1].addr, tokens[1].len, tokens[2].addr);

    return (IRC_INVITE);
}
