#include <ctype.h>
#include "server/irc.h"

void irc_quit(t_env *e, int cs, t_token *tokens)
{
    char concat[512];

    memset(concat, 0, sizeof(concat));

    time2iso(e->isotime);

    if (tokens[1].addr)
        sprintf(concat, "%s quit: %s", e->fds[cs].nickname, tokens[1].addr);
    else
        sprintf(concat, "%s quit.", e->fds[cs].nickname);

    broadcast(e, concat, IRC_INFO, cs);

    close(cs);
    clear_fd(&e->fds[cs]);
    printf(e->is_tty ? "\x1b[31m"
                       "Client #%d gone away"
                       "\x1B[0m\n"
                     : "Client #%d gone away\n",
           cs);

    FD_CLR(cs, &e->fd_read);
    FD_CLR(cs, &e->fd_write);
}
