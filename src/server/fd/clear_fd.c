#include <server/irc.h>

void clear_fd(t_fd *fd)
{
    fd->registered = 0;
    fd->away = 0;
    fd->type = FD_FREE;
    fd->read = NULL;
    fd->write = NULL;
}
