#include <client/irc.h>
#include <ctype.h>

int _c2s_away(t_fd *fd, const char *msg, size_t msg_len)
{
    logdebug("_c2s_away:: %.*s\n", msg_len, msg);

    return (cbuffer_putcmd(&fd->buf_write, "AWAY %.*s\x0D\x0A", msg_len, msg));
}

int c2s_away(t_env *e, int cs, t_token *tokens)
{
    if (e->sock == -1)
        return logerror("%s\n",
                        "You need to be logged in before any command. Use "
                        "/connect [server] ?[port]");

    if (tokens[1].addr == (void *)0)
        return (irc_error(e, ERR_NEEDMOREPARAMS, tokens[0].addr));

    _c2s_away(&e->fds[cs], tokens[1].addr, strlen(tokens[1].addr));

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
