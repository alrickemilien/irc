#include <client/irc.h>
#include <client/ui/panel.h>
#include <ctype.h>

static int c2s_nick_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t nick_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr || tokens[2].addr)
        return (irc_error(e, ERR_NONICKNAMEGIVEN));

    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len)
        return (irc_error(e, ERR_ERRONEUSNICKNAME, tokens[1].addr));

    return (0);
}

int _c2s_nick(t_env *e, const char *nick, size_t nick_length)
{
    // Set only local nickname
    if (e->sock == -1)
    {
        memrpl(e->nick, NICKNAMESTRSIZE, nick, nick_length);
        return (0);
    }

    if (cbuffer_putcmd(&e->fds[e->sock].buf_write, "NICK %.*s\x0D\x0A",
                       nick_length, nick) < 0)
        return (-1);

    loginfo("You changed nickname to %s", nick);

    memrpl(e->fds[e->sock].nickname, NICKNAMESTRSIZE, nick, nick_length);

    return (0);
}

int c2s_nick(t_env *e, int cs, t_token *tokens)
{
    if ((c2s_nick_check_command(e, cs, tokens)) < 0)
        return (-1);

    _c2s_nick(e, tokens[1].addr, tokens[1].len);

    if (e->options.gui)
        ui_set_nick(e->ui, tokens[1].addr);

    return (IRC_NICK);
}
