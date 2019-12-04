#include <client/irc.h>
#include <client/ui/panel.h>

static int s2c_nick_check_command(t_env *e, const t_token *tokens)
{
    size_t nick_len;

    if (!tokens[0].addr || !tokens[1].addr || tokens[2].addr)
        return (irc_error(e, ERR_NONICKNAMEGIVEN));

    nick_len = tokens[2].len;

    if (nick_len > 9 || !nick_len)
        return (irc_error(e, ERR_ERRONEUSNICKNAME, tokens[2].addr));

    return (0);
}

/*
** Return true when the JOIN user in the message is the current client
*/

static bool s2c_nick_is_me(t_env *e, const char *nick, size_t len)
{
    return (strncmp(e->self->nickname, nick, len) == 0);
}

int s2c_nick(t_env *e, t_token *tokens)
{
    logdebug("s2c_nick:: %s", tokens[0].addr);

    if (s2c_nick_check_command(e, tokens) < 0)
        return (-1);

    if (s2c_nick_is_me(
            e, tokens[0].addr[0] == ':' ? tokens[0].addr + 1 : tokens[0].addr,
            tokens[0].addr[0] == ':' ? tokens[0].len - 1 : tokens[0].len))
    {
        memrpl(e->fds[e->sock].nickname, NICKNAMESTRSIZE, tokens[2].addr,
               tokens[2].len);
        loginfo("s2c_nick_is_me:: You changed nickname to %s",
                e->fds[e->sock].channelname);
        if (e->options.gui)
            ui_set_nick(e->ui, tokens[2].addr);
    }

    return (IRC_S2C_NICK);
}
