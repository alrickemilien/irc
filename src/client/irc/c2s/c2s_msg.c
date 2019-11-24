#include <client/irc.h>
#include <ctype.h>

static int c2s_msg_check_command(t_env *e, int cs, const t_token *tokens)
{
    (void)cs;
    (void)e;

    if (!tokens[1].addr || !tokens[2].addr)
        return logerror("c2s_msg_check_command::ERR_NEEDMOREPARAMS\n");
    return (0);
}

int _c2s_msg(t_fd *fd, const char *dest, size_t dest_len, const char *msg)
{
    logdebug("_c2s_msg:: PRIVMSG: %s :%s\n", dest, msg);
    return (cbuffer_putcmd(&fd->buf_write, "PRIVMSG %*s :%s\x0D\x0A", dest_len,
                           dest, msg));
}

int c2s_msg(t_env *e, int cs, t_token *tokens)
{
    if (e->sock == -1)
        return logerror(
            "%s\n",
            "You nee to be logged in before any command. Use /connect [server] "
            "?[port]");

    if ((c2s_msg_check_command(e, cs, tokens)) < 0)
        return (-1);

    _c2s_msg(&e->fds[cs], tokens[1].addr, tokens[1].len, tokens[2].addr);

    return (IRC_JOIN);
}
