#include "irc.h"

void do_select(t_env *e)
{
    // return the number of file descriptors
    // contained in the three returned descriptor sets
    e->r = xsafe(
        -1, select(e->max + 1, &e->fd_read, &e->fd_write, (void *)0, (void *)0),
        "do_select::select");
}