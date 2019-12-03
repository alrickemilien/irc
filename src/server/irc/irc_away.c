#include <server/irc.h>

int irc_away(t_env *e, int cs, t_token *tokens)
{
    if (!tokens[1].addr)
    {
        memset(e->fds[cs].awaymessage, 0, sizeof(e->fds[cs].awaymessage));
        e->fds[cs].away = 0;
        irc_reply(e, cs, RPL_UNAWAY, NULL);
        loginfo("%s marked as not away\n", e->fds[cs].nickname);
        return (IRC_AWAY);
    }

    memrpl(e->fds[cs].awaymessage, sizeof(e->fds[cs].awaymessage),
           tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr,
           tokens[1].addr[0] == ':' ? strlen(tokens[1].addr) - 1
                                    : strlen(tokens[1].addr));
    e->fds[cs].away = 1;
    irc_reply(e, cs, RPL_NOWAWAY, NULL);
    loginfo("%s marked as away\n", e->fds[cs].nickname);

    return (IRC_AWAY);
}
