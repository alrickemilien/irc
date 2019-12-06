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

#define IRC_SERVER_VERSION "1.0.0"
#define IRC_SERVER_VERSION_COMMENT "Basic version IRC server @ 42"

#define PIDFILE "./ircserver.pid"

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

#define DEFAULT_CHANNEL "&hub"
#define DEFAULT_NICKNAME "Ben_AFK"

typedef struct s_env
{
    t_fd *     fds;
    t_channel *channels;
    int        port;
    size_t     maxfd;
    size_t     maxchannels;
    size_t     max;
    int        r;
    fd_set     fd_read;
    fd_set     fd_write;
    char       isotime[ISOTIMESTRSIZE];
    void *     ssl_ctx;
} t_env;

/*
** fd
*/

void clear_fd(t_fd *fd);
void init_fd(t_env *e);
void check_fd(t_env *e);

/*
** irc sepcific to server
*/

typedef enum e_irc {
    IRC_JOIN = 0UL,
    IRC_NICK,
    IRC_USER,
    IRC_QUIT,
    IRC_NAMES,
    IRC_PRIVMSG,
    IRC_PASS,
    IRC_AWAY,
    IRC_PART,
    IRC_WHOIS,
    IRC_WHO,
    IRC_NOTICE,
    IRC_LIST,
    IRC_TIME,
    IRC_PING,
    IRC_PONG,
    IRC_VERSION,
    IRC_INVITE,
    IRC_TOPIC,
    IRC_COMMANDS_NUMBER
} t_irc_enum;

typedef struct s_irc_cmd
{
    char *command;
    int (*f)(t_env *e, int cs, t_token *tokens);
} t_irc_cmd;

int irc_command(t_env *e, int cs, size_t end_command_index);
int irc_join(t_env *e, int cs, t_token *tokens);
int irc_nick(t_env *e, int cs, t_token *tokens);
int irc_user(t_env *e, int cs, t_token *tokens);
int irc_quit(t_env *e, int cs, t_token *tokens);
int irc_names(t_env *e, int cs, t_token *tokens);
int irc_notice(t_env *e, int cs, t_token *tokens);
int irc_away(t_env *e, int cs, t_token *tokens);
int irc_privmsg(t_env *e, int cs, t_token *tokens);
int irc_pass(t_env *e, int cs, t_token *tokens);
int irc_part(t_env *e, int cs, t_token *tokens);
int irc_whois(t_env *e, int cs, t_token *tokens);
int irc_who(t_env *e, int cs, t_token *tokens);
int irc_list(t_env *e, int cs, t_token *tokens);
int irc_time(t_env *e, int cs, t_token *tokens);
int irc_ping(t_env *e, int cs, t_token *tokens);
int irc_pong(t_env *e, int cs, t_token *tokens);
int irc_version(t_env *e, int cs, t_token *tokens);
int irc_invite(t_env *e, int cs, t_token *tokens);
int irc_topic(t_env *e, int cs, t_token *tokens);
int irc_reply(t_env *e, int cs, int code, ...);
int irc_reply_all_in_channel(t_env *e, int cs, int code, ...);
int irc_err(t_env *e, int cs, int code, ...);

void irc_user_join_default_channel(t_env *e, int cs);
void irc_privmsg_nomatch_nick(t_env *  e,
                              int      cs,
                              t_token *subtokens,
                              size_t   subtoken_count);

int ping(t_env *e, int cs);
void broadcast(t_env *e, const char *msg, int type, size_t cs);
void broadcast_all_in_channel(t_env *     e,
                              const char *msg,
                              int         msg_type,
                              size_t      cs);
void disconnect(t_env *e, size_t cs);

void serv(t_env *e);

int on_connect(t_env *e, size_t sock);
int server_ipv6(const t_options *options, t_env *e);
int server_ipv4(const t_options *options, t_env *e);

int client_write(t_env *e, size_t cs);
int client_read(t_env *e, size_t cs);

int do_select(t_env *e);
int daemonize(void);

#endif
