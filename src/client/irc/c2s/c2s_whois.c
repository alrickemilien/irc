#include <client/irc.h>
#include <client/ui/panel.h>

static int c2s_whois_check_command(t_env *e, const t_token *tokens)
{
    size_t nick_len;

    if (!tokens[1].addr)
        return (irc_error(e, ERR_NONICKNAMEGIVEN));

    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len)
        return (irc_error(e, ERR_ERRONEUSNICKNAME, tokens[1].addr));

    return (0);
}

int _c2s_whois(t_fd *fd, const char *nick, size_t nick_len)
{
    return (
        cbuffer_putcmd(&fd->buf_write, "WHOIS %.*s\x0D\x0A", nick_len, nick));
}

int c2s_whois(t_env *e, t_token *tokens)
{
    if (e->sock == -1)
        return (irc_error(e, ERR_NOT_CONNECTED));

    if (c2s_whois_check_command(e, tokens) < 0)
        return (-1);

    if (_c2s_whois(e->self, tokens[1].addr, tokens[1].len) < 0)
        return (-1);

    return (IRC_WHO);
}
