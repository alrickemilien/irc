#include <ctype.h>
#include <server/irc.h>

static int irc_pass_check_command(t_env *e, int cs, const t_token *tokens)
{
    if (e->fds[cs].registered)
        return (irc_err(e, cs, ERR_ALREADYREGISTRED, NULL));
    if (!tokens[1].addr)
        return (irc_err(e, cs, ERR_NEEDMOREPARAMS, "PASS"));
    return (0);
}

int irc_pass(t_env *e, int cs, t_token *tokens)
{
    if ((irc_pass_check_command(e, cs, tokens)) != 0)
        return (-1);

    memrpl(e->fds[cs].passwd, PASSWDTRSIZE, tokens[1].addr, tokens[1].len);

    return (IRC_PASS);
}
