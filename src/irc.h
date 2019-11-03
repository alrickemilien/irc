#ifndef IRC_H
#define IRC_H

#include <sys/select.h>
#include <stdlib.h>

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

#define BUF_SIZE 4096

#define MAX(a, b) ((a > b) ? a : b)

int xsafe(int err, int res, char *str);
void *xpsafe(void *err, void *res, char *str);

#define XPSAFE(err, res, str) (xpsafe(err, res, str))
#define XSAFE(err, res, str) (xsafe(err, res, str))

typedef struct s_fd
{
    int type;
    void (*read)();
    void (*write)();
    char buf_read[BUF_SIZE + 1];
    char buf_write[BUF_SIZE + 1];
} t_fd;

typedef struct s_env
{
    t_fd * fds;
    int    port;
    size_t maxfd;
    size_t max;
    int    r;
    fd_set fd_read;
    fd_set fd_write;
} t_env;

void serv(t_env *e);

void on_connect(t_env *e, size_t sock);
void server_create(t_env *e, int port);
int create_server_ipv6(t_env *e, int port);

void client_write(t_env *e, size_t cs);
void client_read(t_env *e, size_t cs);

void clear_fd(t_fd *fd);
void init_fd(t_env *e);
void check_fd(t_env *e);

void do_select(t_env *e);
#endif