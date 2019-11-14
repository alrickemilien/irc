#ifndef CLIENT_IRC_H
#define CLIENT_IRC_H

#include <stdlib.h>
#include <sys/select.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

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
}               t_env;

/*
** irc sepcific to server
*/

typedef enum e_irc {
    IRC_JOIN = 0UL,
    IRC_NICK,
    IRC_MSG,
    IRC_COMMANDS_NUMBER
} t_irc_enum;

typedef struct  s_irc_cmd
{
    char        *command;
    int         (*f)(t_env *e, int cs, t_token *tokens);
}               t_irc_cmd;

int             irc_command(t_env *e, int cs, char *buffer);
int             irc_join(t_env *e, int cs, t_token *tokens);
int             irc_msg(t_env *e, int cs, t_token *tokens);
int             irc_nick(t_env *e, int cs, t_token *tokens);

/*
** fd 
*/

void            clear_fd(t_fd *fd);
void            init_fd(t_env *e);
void            check_fd(t_env *e);

void            client_ipv4(const t_options *options, t_env *e);
void            client_ipv6(const t_options *options, t_env *e);

void            server_read(t_env *e, size_t cs);
void            server_write(t_env *e, size_t cs);

void            do_select(const t_options *options, t_env *e);
#endif
