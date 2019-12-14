#include <client/irc.h>

static int c2s_msg_check_command(t_env *e, const t_token *tokens)
{
    if (!tokens[1].addr || !tokens[2].addr)
        return (irc_error(e, ERR_NEEDMOREPARAMS, tokens[0].addr));
    return (0);
}

int _c2s_msg(t_fd *fd, const char *dest, size_t dest_len, const char *msg)
{
    logdebug("PRIVMSG %.*s :%s\x0D\x0A", dest_len, dest, msg);
    return (cbuffer_putcmd(&fd->buf_write, "PRIVMSG %.*s :%s\x0D\x0A", dest_len,
                           dest, msg));
}

int c2s_msg(t_env *e, t_token *tokens)
{
    if (e->sock == -1)
        return (irc_error(e, ERR_NOT_CONNECTED));

    if ((c2s_msg_check_command(e, tokens)) < 0)
        return (-1);

    _c2s_msg(e->self, tokens[1].addr, tokens[1].len, tokens[2].addr);

    return (IRC_C2S_MSG);
}
