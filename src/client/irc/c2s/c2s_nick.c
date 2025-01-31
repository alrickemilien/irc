#include <client/irc.h>
#include <client/ui/panel.h>

static int c2s_nick_check_command(t_env *e, const t_token *tokens)
{
    if (!tokens[1].addr)
        return (irc_error(e, ERR_NONICKNAMEGIVEN));

    if (tokens[2].addr)
        return (irc_error(e, ERR_NICK_BAD_FORMAT));

    if (tokens[1].len > 9 || !tokens[1].len || !is_valid_nick(tokens[1].addr))
        return (irc_error(e, ERR_ERRONEUSNICKNAME, tokens[1].addr));

    return (0);
}

int _c2s_nick(t_env *e, const char *nick, size_t nick_length)
{
    memrpl(e->nick, NICKNAMESTRSIZE, nick, nick_length);

    // Set only local nickname
    if (e->sock == -1)
        return (0);

    if (cbuffer_putcmd(&e->self->buf_write, "NICK %.*s\x0D\x0A",
                       nick_length, nick) < 0)
        return (-1);

    loginfo("You changed nickname to %s", nick);

    memrpl(e->self->nickname, NICKNAMESTRSIZE, nick, nick_length);

    return (0);
}

int c2s_nick(t_env *e, t_token *tokens)
{
    if ((c2s_nick_check_command(e, tokens)) < 0)
        return (-1);

    _c2s_nick(e, tokens[1].addr, tokens[1].len);

    if (e->options.gui)
        ui_set_nick(e->ui, tokens[1].addr);

    return (IRC_C2S_NICK);
}
