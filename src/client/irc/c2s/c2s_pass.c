#include <client/irc.h>
#include <ctype.h>

static int c2s_pass_check_command(t_env *e, int cs, const t_token *tokens)
{
    (void)cs;
    (void)e;

    if (!tokens[1].addr)
        return (logerror("c2s_pass_check_command::Missing password\n"));

    if (tokens[1].len > 200 || !tokens[1].len)
        return (logerror("c2s_pass_check_command::Missing password\n"));

    return (0);
}

int c2s_pass(t_env *e, int cs, t_token *tokens)
{
    if (e->sock == -1)
        return logerror("You already logged in\n");

    if ((c2s_pass_check_command(e, cs, tokens)) < 0)
        return (-1);

    cbuffer_putstr(&e->fds[cs].buf_write, "PASS ");
    cbuffer_put(&e->fds[cs].buf_write, (uint8_t *)tokens[1].addr,
                tokens[1].len);
    cbuffer_putstr(&e->fds[cs].buf_write, "\x0D\x0A");

    loginfo("You changed password\n");

    memset(e->fds[cs].passwd, 0, PASSWDTRSIZE);
    memcpy(e->fds[cs].passwd, tokens[1].addr, tokens[1].len);

    return (IRC_PASS);
}
