#include "server/irc.h"

void broadcast(t_env *e, const char *msg, int msg_type, size_t cs)
{
    size_t i;

    printf("BROADCASTING\n");

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT &&
            !strcmp(e->fds[i].channel, e->fds[cs].channel))
        {
            if (msg_type == IRC_INFO && i != cs)
                strcat(e->fds[i].buf_write, msg);
            else if (i != cs)
            {
                strcat(e->fds[i].buf_write, "ALERT: ");
                strcat(e->fds[i].buf_write, e->fds[cs].nickname);
                strcat(e->fds[i].buf_write, msg);
            }
        }
        i++;
    }
}
