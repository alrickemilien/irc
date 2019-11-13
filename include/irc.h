#ifndef IRC_H
#define IRC_H

/*
** This file contains all shared prototypes, macros, enum, ...
** between server and clients
*/

#include <stdint.h>
#include <stddef.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/resource.h>

/*
** Utils
*/

#define MAX(a, b) ((a > b) ? a : b)

#define TOSTR(x) #x

int xsafe(int err, int res, char *str);
#define XSAFE(err, res, str) (xsafe(err, res, str))
void *xpsafe(void *err, void *res, char *str);
#define XPSAFE(err, res, str) (xpsafe(err, res, str))

#define ISOTIMESTRSIZE 25
void time2iso(char *str);

int ato32(const char *str, uint32_t *nbr);
int i64toa(uint64_t nbr, char *buffer, size_t buffer_size, uint64_t base);

/*
** log
*/

#include <stdarg.h>
#include <time.h>

int loginfo(const char *fmt, ...);
int logerror(const char *fmt, ...);
int logerrno(const char *str);

/*
** IRC specific
*/

#define BUF_SIZE 4096
#define CHANNELSTRSIZE 200
#define NICKNAMESTRSIZE 9
#define HOSTNAMESTRSIZE NI_MAXHOST
#define USERNAMESTRSIZE 20
#define PASSWDTRSIZE 512
#define MAXMSGSIZE 512

/*
** Tokenize
*/

typedef struct s_token
{
    char * addr;
    size_t len;
} t_token;

size_t tokenize(char *str, t_token *tokens, size_t len);
size_t tokenizechr(char *str, t_token *tokens, size_t len, int c);

/*
** CBuffer
*/

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

/*
** File descriptor
*/

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

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
    char host[NI_MAXHOST + 1];  // the real name of the host that the
                                // client is running on
    char serv[NI_MAXSERV + 1];
    char username[USERNAMESTRSIZE + 1];  // the username on that host
    char realname[USERNAMESTRSIZE + 1];  // the username on that host
    char passwd[PASSWDTRSIZE + 1];       // the username on that host
    char awaymessage[BUF_SIZE + 1];      // the message to send when away
    int  chop;
    int  registered;
    int  away;
} t_fd;

/*
** Replys and errors
*/

enum e_irc_reply
{
    RPL_WELCOME = 001,
    RPL_AWAY = 301,
    RPL_UNAWAY = 305,
    RPL_NOWAWAY = 306,
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_NOTEXTTOSEND = 412,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
};

typedef struct s_irc_reply
{
    int   code;
    char *name;
    char *fmt;
} t_irc_reply;

#endif