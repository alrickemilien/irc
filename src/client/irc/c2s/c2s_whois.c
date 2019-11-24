#include <client/irc.h>
#include <ctype.h>

static int c2s_whois_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t nick_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr)
        return (logerror("c2s_whois_check_command::ERR_NONICKNAMEGIVEN\n"));

    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len)
        return (logerror("c2s_whois_check_command::ERR_ERRONEUSNICKNAME\n"));

    return (0);
}

int _c2s_whois(t_fd *fd, const char *nick, size_t nick_len)
{
    cbuffer_putstr(&fd->buf_write, "WHOIS ");
    cbuffer_put(&fd->buf_write, (uint8_t*)nick, nick_len);
    cbuffer_putstr(&fd->buf_write, "\x0D\x0A");
    return (0);
}

int c2s_whois(t_env *e, int cs, t_token *tokens)
{
    loginfo("c2s_whois:: %s\n", tokens[0].addr);

    if (e->sock == -1)
        return logerror(
            "%s\n",
            "You nee to be logged in before any command. Use /connect [server] "
            "?[port]");

    if (c2s_whois_check_command(e, cs, tokens) < 0)
        return (-1);

    _c2s_whois(&e->fds[cs], tokens[1].addr, tokens[1].len);

    return (IRC_WHO);
}
