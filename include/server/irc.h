#ifndef SERVER_IRC_H
#define SERVER_IRC_H

#include <stdlib.h>
#include <sys/select.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <irc.h>
#include <server/options.h>

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

#define BUF_SIZE 4096
#define CHANNELSTRSIZE 140
#define NICKNAMESTRSIZE 140

typedef struct s_fd
{
    int type;
    void (*read)();
    void (*write)();
    char buf_read[BUF_SIZE + 1];
    char buf_write[BUF_SIZE + 1];

    // User data
    char channel[CHANNELSTRSIZE + 1];
    char nickname[NICKNAMESTRSIZE + 1];
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
    char   isotime[ISOTIMESTRSIZE];
    int    is_tty;
} t_env;

typedef enum e_irc { IRC_JOIN = 0UL, IRC_COMMANDS_NUMBER } t_irc_enum;

typedef struct s_irc_cmd
{
    char *command;
    void (*f)(t_env *e, int cs, char **command);
} t_irc_cmd;

void irc_command(t_env *e, int cs, char *buffer);
void irc_join(t_env *e, int cs, char **buffer);
size_t tokenize(char *str, char **tokens, size_t len);

// Broadcast messages' types
#define IRC_INFO 42

void broadcast(t_env *e, const char *msg, int type, size_t cs);

void serv(t_env *e);

void on_connect(t_env *e, size_t sock);
void server_create(t_env *e, int port);
int server_ipv6(const t_options *options, t_env *e);
void server_ipv4(const t_options *options, t_env *e);

void client_write(t_env *e, size_t cs);
void client_read(t_env *e, size_t cs);

void clear_fd(t_fd *fd);
void init_fd(t_env *e);
void check_fd(t_env *e);

void do_select(t_env *e);
void daemonize(void);

#endif
