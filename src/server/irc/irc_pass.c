#include <ctype.h>
#include <server/irc.h>

static int irc_pass_check_command(t_env *e, int cs, const t_token *tokens)
{
    if (e->fds[cs].registered)
    {
        irc_reply(e, cs, ERR_ALREADYREGISTRED, NULL);
        return (-1);
    }

    if (!tokens[1].addr)
    {
        irc_reply(e, cs, ERR_NEEDMOREPARAMS, NULL);
        return (-1);
    }

    return (0);
}

int irc_pass(t_env *e, int cs, t_token *tokens)
{
    if ((irc_pass_check_command(e, cs, tokens)) != 0)
        return (-1);

    memset(e->fds[cs].passwd, 0, sizeof(e->fds[cs].passwd));
    memcpy(e->fds[cs].passwd, tokens[1].addr, tokens[1].len);

    return (IRC_PASS);
}
