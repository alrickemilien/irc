#include <client/irc.h>
#include <client/ui/panel.h>
#include <ctype.h>

static int s2c_nick_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t nick_len;

    (void)cs;
    (void)e;

    if (!tokens[0].addr || !tokens[1].addr || tokens[2].addr)
        return (logerror("s2c_nick_check_command:: ERR_NONICKNAMEGIVEN\n"));

    nick_len = tokens[2].len;

    if (nick_len > 9 || !nick_len)
        return (logerror("s2c_nick_check_command:: ERR_ERRONEUSNICKNAME\n"));

    return (0);
}

/*
** Return true when the JOIN user in the message is the current client
*/

static bool s2c_nick_is_me(t_env *e, const char *nick, size_t len)
{
    return (strncmp(e->fds[e->sock].nickname, nick, len) == 0);
}

int s2c_nick(t_env *e, int cs, t_token *tokens)
{
    logdebug("s2c_nick:: %s\n", tokens[0].addr);

    if (s2c_nick_check_command(e, cs, tokens) < 0)
        return (-1);

    if (s2c_nick_is_me(
            e, tokens[0].addr[0] == ':' ? tokens[0].addr + 1 : tokens[0].addr,
            tokens[0].addr[0] == ':' ? tokens[0].len - 1 : tokens[0].len))
    {
        memrpl(e->fds[e->sock].nickname, NICKNAMESTRSIZE, tokens[2].addr,
               tokens[2].len);
        loginfo("s2c_nick_is_me:: You changed nickname to %s\n",
                e->fds[e->sock].channelname);
        if (e->options.gui)
            set_nick_name(e->ui, tokens[2].addr);
    }

    return (IRC_S2C_NICK);
}
