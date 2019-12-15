#include <server/irc.h>

/*
** In the absence of the <name> parameter, all visible
** (users who aren't invisible (user mode +i) and who don't have a
** common channel with the requesting client) are listed.
** The same result can be achieved by using a <name> of "0" or any wildcard
** which
** will end up matching every entry possible.
*/

int irc_who_all(t_env *e, int cs, t_token *tokens)
{
    size_t i;

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1 &&
            strcmp(e->channels[e->fds[cs].channel].channel,
                   e->channels[e->fds[i].channel].channel) != 0)
        {
            irc_reply(e, cs, RPL_WHOREPLY,
                      e->channels[e->fds[i].channel].channel,
                      e->fds[i].username, "server", e->fds[i].host,
                      e->fds[i].nickname, e->fds[i].realname);

            irc_reply(e, cs, RPL_ENDOFWHO, tokens[1].addr);
        }
        i++;
    }
    return (IRC_WHO);
}

int irc_who(t_env *e, int cs, t_token *tokens)
{
    size_t i;

    if (!tokens[1].addr ||
        (tokens[1].len == 1 && strncmp(tokens[1].addr, "*", 1)) ||
        (tokens[1].len == 1 && strncmp(tokens[1].addr, "0", 1)))
        irc_who_all(e, cs, tokens);

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1)
        {
            if (strncmp(e->fds[i].host, tokens[1].addr, tokens[1].len) == 0
                /*|| strncmp(e->hostname , tokens[1].addr, tokens[1].len) == 0
                 */
                || strncmp(e->fds[i].realname, tokens[1].addr, tokens[1].len) ==
                       0 ||
                strncmp(e->fds[i].nickname, tokens[1].addr, tokens[1].len) == 0)
            {
                irc_reply(e, cs, RPL_WHOREPLY,
                          e->channels[e->fds[i].channel].channel,
                          e->fds[i].username, "server", e->fds[i].host,
                          e->fds[i].nickname, e->fds[i].realname);

                irc_reply(e, cs, RPL_ENDOFWHO, tokens[1].addr);

                return (IRC_WHO);
            }
        }
        i++;
    }

    irc_reply(e, cs, ERR_NOSUCHNICK, tokens[1].addr);

    return (IRC_WHO);
}
