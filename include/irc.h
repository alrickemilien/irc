#ifndef IRC_H
#define IRC_H

/*
** This file contains all shared prototypes, macros, enum, ...
** between server and clients
*/

#include <netdb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/resource.h>
#include <sys/time.h>

// SO_NOSIGPIPE compat OSX
// See https://github.com/lpeterse/haskell-socket/issues/8#issuecomment-115650974
#ifdef __APPLE__
#define MSG_NOSIGNAL 0
#endif

/*
** Utils
*/

#ifndef MAX
# define MAX(a, b) ((a > b) ? a : b)
#endif


#define TOSTR(x) #x

int xsafe(int err, int res, char *str);
#define XSAFE(err, res, str) (xsafe(err, res, str))
void *xpsafe(void *err, void *res, char *str);
#define XPSAFE(err, res, str) (xpsafe(err, res, str))

#define ISOTIMESTRSIZE 25
void time2iso(char *str);

int   ato32(const char *str, uint32_t *nbr);
int   i64toa(uint64_t nbr, char *buffer, size_t buffer_size, uint64_t base);
char *extract_folder_from_path(const char *path);
char *merge_and_extract_folder_from_path(const char *a, const char *b);
char *strjoin(char const *s1, char const *s2);
void *memrpl(char *dest, size_t dest_size, const char *src, size_t src_size);

/*
** irc utils
*/
bool is_valid_chan_name(const char *channel);

/*
** log
*/

#include <stdarg.h>
#include <time.h>

int loginfo(const char *fmt, ...);
int logerror(const char *fmt, ...);
int logerrno(const char *str);
int logdebug(const char *fmt, ...);

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

#include <cbuffer/cbuffer.h>

/*
** File descriptor
*/

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

typedef struct s_fd
{
    int type;
    int (*read)();
    int (*write)();
    t_cbuffer buf_read;
    t_cbuffer buf_write;

    // User data
    size_t channel;
    char   nickname[NICKNAMESTRSIZE + 1];
    char   host[NI_MAXHOST + 1];  // the real name of the running on
    char   serv[NI_MAXSERV + 1];
    char   username[USERNAMESTRSIZE + 1];    // the username on that host
    char   realname[USERNAMESTRSIZE + 1];    // the username on that host
    char   passwd[PASSWDTRSIZE + 1];         // the username on that host
    char   awaymessage[BUF_SIZE + 1];        // the message to send when away
    char   channelname[CHANNELSTRSIZE + 1];  // the message to send when away
    int    registered;
    int    away;
    void * ssl;
} t_fd;

/*
** Channel
*/

typedef struct s_channel
{
    char   channel[CHANNELSTRSIZE + 1];
    int    chop;
    size_t clients;
} t_channel;

/*
** Replys and errors
*/

enum e_irc_reply
{
    RPL_WELCOME = 001,
    RPL_AWAY = 301,
    RPL_UNAWAY = 305,
    RPL_NOWAWAY = 306,
    RPL_WHOISUSER = 311,
    RPL_ENDOFWHO = 315,
    RPL_ENDOFWHOIS = 318,
    RPL_WHOISCHANNELS = 319,
    RPL_TOPIC = 332,
    RPL_WHOREPLY = 352,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_NOTEXTTOSEND = 412,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NOTONCHANNEL = 442,
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
