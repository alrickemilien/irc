#include <client/irc.h>

static int	c2s_pass_check_command(
	t_env *e, const t_token *tokens)
{
    if (!tokens[1].addr || !tokens[1].len)
        return (irc_error(e, ERR_NEEDMOREPARAMS));
    if (tokens[1].len > 200)
        return (logerror("Missing password"));
    return (0);
}

int			do_c2s_pass(
	t_env *e, const char *password,
	size_t password_length)
{
    memrpl(e->passwd, PASSWDTRSIZE, password, password_length);
    return (0);
}

int			c2s_pass(t_env *e, t_token *tokens)
{
    if (e->sock != -1)
        return logerror("You are already logged in");
    if ((c2s_pass_check_command(e, tokens)) < 0)
        return (-1);
    if (do_c2s_pass(e, tokens[1].addr, tokens[1].len) < 0)
        return (-1);
    if (cbuffer_putcmd(&e->self->buf_write, "PASS %.*s\x0D\x0A",
                       tokens[1].addr, tokens[1].len) < 0)
        return (-1);
    loginfo("do_c2s_pass::You changed password");
    return (IRC_C2S_PASS);
}
