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
// https://github.com/lpeterse/haskell-socket/issues/8#issuecomment-115650974
#ifdef __APPLE__
#define MSG_NOSIGNAL 0
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

/*
** Utils
*/

#ifndef MAX
#define MAX(a, b) ((a > b) ? a : b)
#endif

#define TOSTR(x) #x

#define ISOTIMESTRSIZE 25
int    time2iso(char *str);
int    ato32(const char *str, uint32_t *nbr);
int    i64toa(uint64_t nbr, char *buffer, size_t buffer_size, uint64_t base);
char * extract_folder_from_path(const char *path);
char * strjoin(char const *s1, char const *s2);
void * memrpl(char *dest, size_t dest_size, const char *src, size_t src_size);
int    fmttime(char *str, const char *fmt);
size_t strlentrim(const char *str);
const char *strtrim(const char *str);

/*
** irc utils
*/

bool is_valid_chan_name(const char *channel, size_t len);
bool is_valid_nick(const char *nick);

/*
** log
*/

#include <stdarg.h>
#include <time.h>

#define LOGSIZE 1024

int loginfo(const char *fmt, ...);
int logerror(const char *fmt, ...);
int logerrno(const char *str);
int logdebug(const char *fmt, ...);

/*
** IRC specific
*/

// #define BUF_SIZE 4096
#define PORTSTRSIZE 5
#define CHANNELSTRSIZE 200
#define NICKNAMESTRSIZE 9
#define HOSTNAMESTRSIZE NI_MAXHOST
#define USERNAMESTRSIZE 20
#define PASSWDTRSIZE 512
#define MAXMSGSIZE 512
#define TOPICSTRSIZE 504
#define IRC_DEFAULT_SERVER_PORT 5555
#define AWAYMSGSIZE 505

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

/*
** Description of the t_fd structure
** char   nickname;
** char   host;         // the real name of the running on
** char   serv;
** char   username;     // the username on that host
** char   realname;     // the username on that host
** char   passwd;       // the username on that host
** char   awaymessage;  // the message to send when away
** char   channelname;  // the message to send when away
*/

typedef struct s_fd
{
    int type;
    int (*read)();
    int (*write)();
    t_cbuffer buf_read;
    t_cbuffer buf_write;

    size_t channel;
    char   nickname[NICKNAMESTRSIZE + 1];
    char   host[NI_MAXHOST + 1];
    char   serv[NI_MAXSERV + 1];
    char   username[USERNAMESTRSIZE + 1];
    char   realname[USERNAMESTRSIZE + 1];
    char   passwd[PASSWDTRSIZE + 1];
    char   awaymessage[AWAYMSGSIZE + 1];
    char   channelname[CHANNELSTRSIZE + 1];
    int    registered;
    int    away;
    long   last_activity;
    long   last_ping_activity;
    void * ssl;
} t_fd;

/*
** Channel
*/

typedef struct s_channel
{
    char   channel[CHANNELSTRSIZE + 1];
    char   topic[TOPICSTRSIZE + 1];
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
    RPL_LISTSTART = 321,
    RPL_LIST = 322,
    RPL_LISTEND = 323,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_INVITING = 341,
    RPL_VERSION = 351,
    RPL_WHOREPLY = 352,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
    RPL_TIME = 391,
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_NOORIGIN = 409,
    ERR_NOTEXTTOSEND = 412,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL = 443,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    ERR_CHANOPRIVSNEEDED = 482,
};

typedef struct s_irc_reply
{
    int   code;
    char *name;
    char *fmt;
} t_irc_reply;

#endif
