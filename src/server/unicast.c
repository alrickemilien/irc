#include <server/irc.h>

void unicast(t_env *e, const char *msg, int msg_type, size_t cs)
{
    if (msg_type == IRC_NOTICE)
    {
        strcat(e->fds[cs].buf_write, "NOTICE ");
        strcat(e->fds[cs].buf_write, msg);
    }
    if (msg_type == IRC_PRIVMSG)
    {
        strcat(e->fds[cs].buf_write, "PRIVMSG ");
        strcat(e->fds[cs].buf_write, msg);
    }
}
