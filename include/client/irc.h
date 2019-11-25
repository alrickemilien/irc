#ifndef CLIENT_IRC_H
#define CLIENT_IRC_H

#include <stdlib.h>
#include <sys/select.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include <irc.h>
#include <client/options.h>

typedef struct  s_env
{
    t_fd        *fds;
    int         port;
    size_t      maxfd;
    size_t      max;
    int         r;
    fd_set      fd_read;
    fd_set      fd_write;
    char        isotime[ISOTIMESTRSIZE];
    int         is_tty;
    int         sock;
    char		cwd[PATH_MAX];
    int         ipv6;
    char        *argv_0;
    char        passwd[PASSWDTRSIZE + 1];
    char        nick[NICKNAMESTRSIZE + 1];
    int         registered;
    void        *ssl_ctx;
    t_options   options;
}               t_env;

/*
** irc sepcific to server
*/

typedef enum    e_irc {
    IRC_JOIN = 0UL,
    IRC_NICK,
    IRC_MSG,
    IRC_CONNECT,
    IRC_WHO,
    IRC_PASS,
    IRC_AWAY,
    IRC_UNAWAY,
    IRC_LEAVE,
    IRC_WHOIS,
    IRC_COMMANDS_NUMBER
}               t_irc_enum;

typedef enum    e_irc_s2c {
    IRC_S2C_RPL_WELCOME = 0UL,
    IRC_S2C_RPL_NAMREPLY,
    IRC_S2C_RPL_ENDOFNAMES,
    IRC_S2C_PRIVMSG,
    IRC_S2C_JOIN,
    IRC_S2C_NICK,
    IRC_S2C_RPL_NOWAWAY,
    IRC_S2C_RPL_UNAWAY,
    IRC_S2C_RPL_TOPIC,
    IRC_S2C_RPL_WHOISUSER,
    IRC_S2C_RPL_WHOISCHANNELS,
    IRC_S2C_RPL_ENDOFWHOIS,
    IRC_S2C_COMMANDS_NUMBER,
}               t_irc_s2c;

typedef struct  s_irc_cmd
{
    char        *command;
    int         (*f)(t_env *e, int cs, t_token *tokens);
}               t_irc_cmd;

int             c2s(t_env *e, int cs, char *buffer);
int             c2s_join(t_env *e, int cs, t_token *tokens);
int             c2s_msg(t_env *e, int cs, t_token *tokens);
int             c2s_connect(t_env *e, int cs, t_token *tokens);
int             c2s_nick(t_env *e, int cs, t_token *tokens);
int             c2s_who(t_env *e, int cs, t_token *tokens);
int             c2s_pass(t_env *e, int cs, t_token *tokens);
int             c2s_away(t_env *e, int cs, t_token *tokens);
int             c2s_unaway(t_env *e, int cs, t_token *tokens);
int             c2s_leave(t_env *e, int cs, t_token *tokens);
int             c2s_whois(t_env *e, int cs, t_token *tokens);

int             _c2s_nick(t_env *e, const char *nick, size_t nick_length);
int             _c2s_pass(t_env *e, const char *password, size_t password_length);
int             _c2s_connect(t_env *e,
                        const char *name,
                        const char *hostname,
                        const char *servername);
int             _c2s_away(t_fd *fd, const char *msg, size_t msg_len);
int             _c2s_unaway(t_fd *fd);
int             _c2s_leave(t_fd *fd, const char *channel_name, size_t channel_name_len);
int             _c2s_join(t_fd *fd, const char *channel_name, size_t channel_name_len);
int             _c2s_whois(t_fd *fd, const char *nick, size_t nick_len);
int             _c2s_msg(t_fd *fd, const char *dest, size_t dest_len, const char *msg);

int             s2c(t_env *e, int cs, char *buffer);
int             s2c_rpl_welcome(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_namreply(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_endofnames(t_env *e, int cs, t_token *tokens);
int             s2c_privmsg(t_env *e, int cs, t_token *tokens);
int             s2c_join(t_env *e, int cs, t_token *tokens);
int             s2c_nick(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_nowaway(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_unaway(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_topic(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_whoisuser(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_whoischannels(t_env *e, int cs, t_token *tokens);
int             s2c_rpl_endofwhois(t_env *e, int cs, t_token *tokens);

/*
** fd 
*/

void            clear_fd(t_fd *fd);
void            init_fd(t_env *e);
void            check_fd(t_env *e);

/*
** Socket
*/

int            client_ipv4(t_env *e);
void            client_ipv6(t_env *e);

int            server_read(t_env *e, size_t cs);
int            server_write(t_env *e, size_t cs);
int            stdin_read(t_env *e, size_t cs);

void            do_select(t_env *e);
#endif
