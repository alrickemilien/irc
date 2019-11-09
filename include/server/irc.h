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
#define CHANNELSTRSIZE 200
#define NICKNAMESTRSIZE 9
#define HOSTNAMESTRSIZE 120
#define USERNAMESTRSIZE 20
#define MAXMSGSIZE 512

#define DEFAULT_CHANNEL "#hub"
#define DEFAULT_NICKNAME "Ben_AFK"

typedef struct s_cbuffer
{
    size_t size;
    char   data[BUF_SIZE + 1];
} t_cbuffer;

int cbuffer_push(t_cbuffer *buffer, char *data, size_t size);
int cbuffer_flush(t_cbuffer *buffer);
int cbuffer_nflush(t_cbuffer *buffer, size_t n);
int cbuffer_recv(t_cbuffer *buffer, int cs);
int cbuffer_pflush(t_cbuffer *buffer, char *data, size_t size);

typedef struct s_fd
{
    int type;
    void (*read)();
    void (*write)();
    t_cbuffer buf_read;
    char      buf_write[BUF_SIZE + 1];

    // User data
    char channel[CHANNELSTRSIZE + 1];
    char nickname[NICKNAMESTRSIZE + 1];
    char hostname[HOSTNAMESTRSIZE + 1];  // the real name of the host that the
                                         // client is running on
    char username[USERNAMESTRSIZE + 1];  // the username on that host
    char realname[USERNAMESTRSIZE + 1];  // the username on that host
    int  chop;
    int  registered;
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

typedef enum e_irc {
    IRC_JOIN = 0UL,
    IRC_MSG,
    IRC_NICK,
    IRC_USER,
    IRC_COMMANDS_NUMBER
} t_irc_enum;

typedef struct s_token
{
    char * addr;
    size_t len;
} t_token;

typedef struct s_irc_cmd
{
    char *command;
    void (*f)(t_env *e, int cs, t_token *tokens);
} t_irc_cmd;

typedef struct s_irc_reply
{
    int   code;
    char *name;
    char *fmt;
} t_irc_reply;

enum e_irc_reply
{
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_NICKNAMEINUSE = 433,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
};

void irc_command(t_env *e, int cs, char *buffer);
void irc_join(t_env *e, int cs, t_token *tokens);
void irc_nick(t_env *e, int cs, t_token *tokens);
void irc_user(t_env *e, int cs, t_token *tokens);
void irc_msg(t_env *e, int cs, t_token *tokens);
int irc_reply(t_env *e, int cs, int code, const char *data);
size_t tokenize(char *str, t_token *tokens, size_t len);

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
