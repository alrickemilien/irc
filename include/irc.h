#ifndef IRC_H
#define IRC_H

/*
** This file contains all shared prototypes, macros, enum, ...
** between server and clients
*/

#include <stdint.h>
#include <stddef.h>

/*
** Utils
*/

#define MAX(a, b) ((a > b) ? a : b)

int xsafe(int err, int res, char *str);
#define XSAFE(err, res, str) (xsafe(err, res, str))
void *xpsafe(void *err, void *res, char *str);
#define XPSAFE(err, res, str) (xpsafe(err, res, str))

#define ISOTIMESTRSIZE 25
void time2iso(char *str);

int ato32(const char *str, uint32_t *nbr);
int i64toa(uint64_t nbr, char *buffer, size_t buffer_size, uint64_t base);

/*
** IRC specific
*/

# define BUF_SIZE 4096
# define CHANNELSTRSIZE 200
# define NICKNAMESTRSIZE 9
# define HOSTNAMESTRSIZE 120
# define USERNAMESTRSIZE 20
# define MAXMSGSIZE 512

/*
** Tokenize
*/

typedef struct s_token
{
    char * addr;
    size_t len;
} t_token;

size_t          tokenize(char *str, t_token *tokens, size_t len);

/*
** CBuffer
*/

typedef struct  s_cbuffer
{
    size_t      size;
    char        data[BUF_SIZE + 1];
}               t_cbuffer;

int             cbuffer_push(t_cbuffer *buffer, char *data, size_t size);
int             cbuffer_flush(t_cbuffer *buffer);
int             cbuffer_nflush(t_cbuffer *buffer, size_t n);
int             cbuffer_recv(t_cbuffer *buffer, int cs);
int             cbuffer_pflush(t_cbuffer *buffer, char *data, size_t size);

/*
** File descriptor
*/

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2
#define FD_TTY 3

typedef struct  s_fd
{
    int         type;
    void        (*read)();
    void        (*write)();
    t_cbuffer   buf_read;
    char        buf_write[BUF_SIZE + 1];

    // User data
    char        channel[CHANNELSTRSIZE + 1];
    char        nickname[NICKNAMESTRSIZE + 1];
    char        hostname[HOSTNAMESTRSIZE + 1];  // the real name of the host that the
                                                // client is running on
    char        username[USERNAMESTRSIZE + 1];  // the username on that host
    char        realname[USERNAMESTRSIZE + 1];  // the username on that host
    int         chop;
    int         registered;
} t_fd;

/*
** Replys and errors
*/

enum e_irc_reply
{
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

typedef struct  s_irc_reply
{
    int         code;
    char        *name;
    char        *fmt;
}               t_irc_reply;

#endif