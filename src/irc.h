#ifndef IRC_H
#define IRC_H

#include <sys/select.h>
#include <stdlib.h>

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

#define BUF_SIZE 4096

int xsafe(int err, int res, char *str);
void *psafe(void *err, void *res, char *str);

#define PSAFE(err, res, str) (psafe(err, res, str))
#define XSAFE(err, res, str) (xsafe(err, res, str))

typedef struct s_fd {
  int type;
  void (*fct_read)();
  void (*fct_write)();
  char buf_read[BUF_SIZE + 1];
  char buf_write[BUF_SIZE + 1];
} t_fd;

typedef struct s_env {
  t_fd *fds;
  int port;
  int maxfd;
  int max;
  int r;
  fd_set fd_read;
  fd_set fd_write;
} t_env;

void    accept_connection(t_env *e, int s);
void    server_create(t_env *e, int port);
void	clearfd(t_fd *fd);

void	client_write(t_env *e, int cs);
void	client_read(t_env *e, int cs);

#endif