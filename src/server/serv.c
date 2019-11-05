#include "server/irc.h"

void serv(t_env *e)
{
    while (1)
    {
        init_fd(e);
        do_select(e);
        check_fd(e);
    }
}