#include "irc.h"

void	clear_fd(t_fd *fd)
{
  fd->type = FD_FREE;
  fd->read = NULL;
  fd->write = NULL;
}
