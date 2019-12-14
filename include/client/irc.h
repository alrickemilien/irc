/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 14:23:40 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 14:23:41 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_IRC_H
# define CLIENT_IRC_H

# include <stdlib.h>
# include <sys/select.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>

# include <locale.h>
# include <libintl.h>
# define LOCALE_DIR "./build/locale/"
# define GETTEXT_PACKAGE "irc"

# include <irc.h>
# include <client/options.h>

typedef struct	s_env
{
	t_fd		*fds;
	t_fd		*self;
	int			port;
	size_t		maxfd;
	size_t		max;
	int			r;
	fd_set		fd_read;
	fd_set		fd_write;
	char		isotime[ISOTIMESTRSIZE];
	int			is_tty;
	int			sock;
	char		cwd[PATH_MAX];
	int			ipv6;
	char		*argv_0;
	char		passwd[PASSWDTRSIZE + 1];
	char		nick[NICKNAMESTRSIZE + 1];
	int			registered;
	void		*ssl_ctx;
	void		*ui;
	t_options	options;
}				t_env;

/*
** Tokenize
*/

typedef struct	s_token
{
	const char	*addr;
	size_t		len;
}				t_token;

size_t			tokenize(
		const char *str, t_token *tokens, size_t len);
size_t			tokenizechr(
		const char *str, t_token *tokens, size_t len, int c);

/*
** irc sepcific to server
*/

typedef enum	e_irc {
	IRC_C2S_JOIN = 0UL,
	IRC_C2S_NICK,
	IRC_C2S_MSG,
	IRC_C2S_CONNECT,
	IRC_C2S_WHO,
	IRC_C2S_PASS,
	IRC_C2S_AWAY,
	IRC_C2S_UNAWAY,
	IRC_C2S_LEAVE,
	IRC_C2S_WHOIS,
	IRC_C2S_LIST,
	IRC_C2S_TIME,
	IRC_C2S_VERSION,
	IRC_C2S_INVITE,
	IRC_C2S_TOPIC,
	IRC_C2S_COMMANDS_NUMBER
}				t_irc_enum;

typedef enum	e_irc_s2c {
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
	IRC_S2C_RPL_WHOREPLY,
	IRC_S2C_RPL_ENDOFWHO,
	IRC_S2C_RPL_AWAY,
	IRC_S2C_RPL_LISTSTART,
	IRC_S2C_RPL_LISTEND,
	IRC_S2C_RPL_LIST,
	IRC_S2C_NOTICE,
	IRC_S2C_RPL_TIME,
	IRC_S2C_PING,
	IRC_S2C_RPL_VERSION,
	IRC_S2C_RPL_INVITING,
	IRC_S2C_INVITE,
	IRC_S2C_RPL_NOTOPIC,
	IRC_S2C_COMMANDS_NUMBER,
}				t_irc_s2c;

/*
** Custom one for internal purpose
*/

typedef enum	e_irc_reply_client {
	ERR_UNRECOGNIZED_COMMAND = 1001,
	ERR_NOT_CONNECTED,
	ERR_NICK_BAD_FORMAT,
}				t_irc_reply_client;

typedef struct	s_irc_cmd
{
	char		*command;
	int			(*f)(t_env *e, t_token *tokens);
}				t_irc_cmd;

typedef struct	s_s2c_error
{
	int			id;
	char		*fmt;
	int			(*f)(
			t_env *e,
			int cs,
			t_token *tokens);
}				t_s2c_error;

int				init_i18n(const char *bin_path);
int				gui(t_env *e, int argc, char **argv);

int				c2s(t_env *e, const char *buffer);
int				c2s_join(t_env *e, t_token *tokens);
int				c2s_msg(t_env *e, t_token *tokens);
int				c2s_connect(t_env *e, t_token *tokens);
int				c2s_nick(t_env *e, t_token *tokens);
int				c2s_who(t_env *e, t_token *tokens);
int				c2s_pass(t_env *e, t_token *tokens);
int				c2s_away(t_env *e, t_token *tokens);
int				c2s_unaway(t_env *e, t_token *tokens);
int				c2s_leave(t_env *e, t_token *tokens);
int				c2s_whois(t_env *e, t_token *tokens);
int				c2s_list(t_env *e, t_token *tokens);
int				c2s_time(t_env *e, t_token *tokens);
int				c2s_version(t_env *e, t_token *tokens);
int				c2s_invite(t_env *e, t_token *tokens);
int				c2s_topic(t_env *e, t_token *tokens);

int				do_c2s_nick(t_env *e, const char *nick, size_t nick_length);
int				do_c2s_pass(
		t_env *e, const char *password, size_t password_length);
int				do_c2s_connect(t_env *e,
		const char *name,
		const char *hostname,
		const char *servername);
int				do_c2s_away(t_fd *fd, const char *msg, size_t msg_len);
int				do_c2s_unaway(t_fd *fd);
int				do_c2s_leave(
		t_fd *fd, const char *channel_name, size_t channel_name_len);
int				do_c2s_join(
		t_fd *fd, const char *channel_name, size_t channel_name_len);
int				do_c2s_whois(
		t_fd *fd, const char *nick, size_t nick_len);
int				do_c2s_msg(
		t_fd *fd, const char *dest, size_t dest_len, const char *msg);
int				do_c2s_invite(
		t_fd *fd, const char *nick, size_t nick_len, const char *channel);
int				do_c2s_topic(
		t_fd *fd, const char *chan, size_t chan_len, const char *msg);

int				s2c(t_env *e, char *buffer);
int				s2c_rpl_welcome(t_env *e, t_token *tokens);
int				s2c_rpl_namreply(t_env *e, t_token *tokens);
int				s2c_rpl_endofnames(t_env *e, t_token *tokens);
int				s2c_privmsg(t_env *e, t_token *tokens);
int				s2c_join(t_env *e, t_token *tokens);
int				s2c_nick(t_env *e, t_token *tokens);
int				s2c_rpl_nowaway(t_env *e, t_token *tokens);
int				s2c_rpl_unaway(t_env *e, t_token *tokens);
int				s2c_rpl_away(t_env *e, t_token *tokens);
int				s2c_rpl_topic(t_env *e, t_token *tokens);
int				s2c_rpl_whoisuser(t_env *e, t_token *tokens);
int				s2c_rpl_whoischannels(t_env *e, t_token *tokens);
int				s2c_rpl_endofwhois(t_env *e, t_token *tokens);
int				s2c_rpl_whoreply(t_env *e, t_token *tokens);
int				s2c_rpl_endofwho(t_env *e, t_token *tokens);
int				s2c_rpl_liststart(t_env *e, t_token *tokens);
int				s2c_rpl_listend(t_env *e, t_token *tokens);
int				s2c_rpl_list(t_env *e, t_token *tokens);
int				s2c_notice(t_env *e, t_token *tokens);
int				s2c_rpl_time(t_env *e, t_token *tokens);
int				s2c_ping(t_env *e, t_token *tokens);
int				s2c_rpl_version(t_env *e, t_token *tokens);
int				s2c_rpl_inviting(t_env *e, t_token *tokens);
int				s2c_invite(t_env *e, t_token *tokens);
int				s2c_rpl_notopic(t_env *e, t_token *tokens);

int				irc_error(t_env *e, int err_code, ...);

/*
** fd
*/

void			clear_fd(t_fd *fd);
void			init_fd(t_env *e);
void			check_fd(t_env *e);

/*
** Socket
*/

int				client_ipv4(t_env *e);
int				client_ipv6(t_env *e);

int				server_read(t_env *e, size_t cs);
int				server_write(t_env *e, size_t cs);
int				stdin_read(t_env *e, size_t cs);

int				do_select(t_env *e);
#endif
