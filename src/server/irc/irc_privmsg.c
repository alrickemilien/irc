#include <ctype.h>
#include "server/irc.h"

static int irc_privmsg_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *nick;
    size_t      nick_len;

    if (!tokens[1].addr)
    {
        irc_reply(e, cs, ERR_NOSUCHNICK, NULL);
        return (-1);
    }

    if (!tokens[2].addr)
    {
        irc_reply(e, cs, ERR_NOTEXTTOSEND, NULL);
        return (-1);
    }

    nick = tokens[1].addr;
    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len)
    {
        irc_reply(e, cs, ERR_NOSUCHNICK, nick);
        return (-1);
    }

    return (0);
}

int irc_privmsg(t_env *e, int cs, t_token *tokens)
{
    size_t i;

    if ((irc_privmsg_check_command(e, cs, tokens)) != 0)
        return (-1);

    // Find client to send private message
    i = 0;
    while (i <= e->max)
    {
        if (i != (size_t)cs && e->fds[i].type == FD_CLIENT &&
            strncmp(e->fds[i].nickname, tokens[1].addr, tokens[1].len) == 0)
        {
            strcat(e->fds[i].buf_write, tokens[2].addr);

            return (IRC_PRIVMSG);
        }
        i++;
    }

    irc_reply(e, cs, ERR_NOSUCHNICK, e->fds[i].nickname);

    return (-1);
}
