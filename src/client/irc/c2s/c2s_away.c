#include <client/irc.h>
#include <ctype.h>

int _c2s_away(t_fd *fd, const char *msg, size_t msg_len)
{
    return (cbuffer_putcmd(&fd->buf_write, "AWAY %.*s\x0D\x0A", msg_len, msg));
}

int c2s_away(t_env *e, int cs, t_token *tokens)
{
    if (e->sock == -1)
        return logerror("%s\n",
                        "You need to be logged in before any command. Use "
                        "/connect [server] ?[port]");

    if (tokens[1].addr == (void *)0)
        return (logerror("c2s_away_check_command::ERR_NEEDMOREPARAMS\n"));

    _c2s_away(&e->fds[cs], tokens[1].addr, tokens[1].len);

    return (IRC_AWAY);
}

int _c2s_unaway(t_fd *fd)
{
    cbuffer_putstr(&fd->buf_write, "AWAY\x0D\x0A");
    return (0);
}

int c2s_unaway(t_env *e, int cs, t_token *tokens)
{
    (void)tokens;

    if (e->sock == -1)
        return logerror("%s\n",
                        "You need to be logged in before any command. Use "
                        "/connect [server] ?[port]");

    _c2s_unaway(&e->fds[cs]);

    return (IRC_UNAWAY);
}
