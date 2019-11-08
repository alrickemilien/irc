#include <ctype.h>
#include "server/irc.h"

static int irc_msg_check_command(t_env *e, int cs, const t_token *tokens)
{
    char *msg;

    msg = tokens[1].addr;

    printf("Received msg : %s\n", msg ? msg : "null");

    if (!msg)
    {
        strcpy(e->fds[cs].buf_write,
               "\x1b[31mERROR\x1b[0m"
               " Usage: /msg blabla bla bla\n");
        return (-1);
    }

    if (strlen(msg) > MAXMSGSIZE)
    {
        strcpy(e->fds[cs].buf_write,
               "\x1b[31mERROR\x1b[0m"
               " Message too long\n");
        return (-1);
    }
    return (0);
}

void irc_msg(t_env *e, int cs, t_token *tokens)
{
    char concat[MAXMSGSIZE + ISOTIMESTRSIZE + NICKNAMESTRSIZE + 5];

    if ((irc_msg_check_command(e, cs, tokens)) != 0)
        return;

    memset(concat, 0, sizeof(concat));

    time2iso(e->isotime);

    sprintf(concat, "[%s] %s: %s\n", e->isotime, e->fds[cs].nickname,
            tokens[1].addr);
    broadcast(e, concat, IRC_INFO, cs);
}
