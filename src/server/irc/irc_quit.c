#include <ctype.h>
#include <server/irc.h>

int irc_quit(t_env *e, int cs, t_token *tokens)
{
    char concat[512];

    memset(concat, 0, sizeof(concat));

    if (tokens[1].addr)
        sprintf(concat, "%s QUIT :%s\x0D\x0A", e->fds[cs].nickname,
                tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr);
    else
        sprintf(concat, "%s QUIT\x0D\x0A", e->fds[cs].nickname);

    broadcast(e, concat, IRC_NOTICE, cs);

    loginfo("%s quit the server\n", e->fds[cs].nickname);

    e->fds[cs].registered = 0;

    return (IRC_QUIT);
}
