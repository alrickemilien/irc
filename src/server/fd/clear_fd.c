#include <server/irc.h>

void clear_fd(t_fd *fd)
{
    fd->type = FD_FREE;
    fd->read = NULL;
    fd->write = NULL;
    memset(fd->buf_write, 0, BUF_SIZE + 1);
    cbuffer_flush(&fd->buf_read);
}
