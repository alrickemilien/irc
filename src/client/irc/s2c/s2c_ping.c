#include <client/irc.h>
#include <client/ui/panel.h>

static int s2c_ping_check_command(t_env *e, const t_token *tokens)
{
    if (!tokens[1].addr)
        return (irc_error(e, ERR_NOORIGIN));
    return (0);
}

/*
** Return true when the JOIN user in the message is the current client
*/

static bool s2c_nick_is_me(t_env *e, const char *nick, size_t len)
{
    return (strncmp(e->self->nickname, nick, len) == 0);
}

int s2c_ping(t_env *e, t_token *tokens)
{
    // logdebug("s2c_ping:: %s", tokens[0].addr);

    if (s2c_ping_check_command(e, tokens) < 0)
        return (-1);

    if (s2c_nick_is_me(e, tokens[1].addr, tokens[1].len))
        return (cbuffer_putcmd(&e->self->buf_write, "PONG\x0D\x0A"));

    return (IRC_S2C_PING);
}
