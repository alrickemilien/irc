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

int _c2s_pass(t_env *e, const char *password, size_t password_length)
{
    memrpl(e->passwd, PASSWDTRSIZE, password, password_length);

    return (0);
}

int c2s_pass(t_env *e, int cs, t_token *tokens)
{
    if (e->sock != -1)
        return logerror("You are already logged in\n");

    if ((c2s_pass_check_command(e, cs, tokens)) < 0)
        return (-1);

    if (_c2s_pass(e, tokens[1].addr, tokens[1].len) < 0)
        return (-1);

    if (cbuffer_putcmd(&e->fds[cs].buf_write, "PASS %*s\x0D\x0A",
                       tokens[1].addr, tokens[1].len) < 0)
        return (-1);

    loginfo("_c2s_pass::You changed password\n");

    return (IRC_PASS);
}
