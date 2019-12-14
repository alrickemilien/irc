/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c2s_connect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 14:16:06 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 14:16:07 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <pwd.h>
#include <sys/types.h>
#include <client/irc.h>

static int	c2s_connect_check_command(
		t_env *e, const t_token *tokens)
{
	size_t host_len;

	if (!tokens[1].addr)
		return (logerror("c2s_connect_check_command:: NO HOST GIVEN\n"));
	host_len = tokens[1].len;
	if (host_len > NI_MAXHOST || !host_len)
		return (logerror("c2s_connect_check_command:: INVALID HOSTNAME\n"));
	memrpl(e->options.host, sizeof(e->options.host), tokens[1].addr,
			tokens[1].len);
	if (!tokens[2].addr)
		return (0);
	if (ato32(tokens[2].addr, (uint32_t *)&e->options.port) != 0 ||
			e->options.port < 1000 || e->options.port > 99999)
	{
		return (logerror("c2s_connect:: port be 1000-99999: '%s'.\n",
			tokens[2].addr));
	}
	return (0);
}

static int	connect_socket(t_env *e)
{
	if (e->ipv6 == 1)
		client_ipv6(e);
	else
		client_ipv4(e);
	if (e->sock == -1)
		return (-1);
	return (0);
}

int			do_c2s_connect(t_env *e,
		const char *n,
		const char *h,
		const char *servername)
{
	char			lh[NI_MAXHOST + 1];
	struct passwd	*p;

	if ((p = getpwuid(getuid())) == NULL)
		return (logerrno("do_c2s_connect::getpwuid"));
	if (gethostname(lh, sizeof(lh)) == -1)
		return (logerrno("do_c2s_connect::gethostname"));
	if (connect_socket(e) == -1)
		return (-1);
	e->self = &e->fds[e->sock];
	cbuffer_putcmd(&e->self->buf_write, "USER %s %s %s %s\x0D\x0A",
		n ? n : p->pw_name, h ? h : lh, servername,
		n ? n : p->pw_name);
	loginfo("Connecting to %s", e->options.host);
	memrpl(e->self->host, HOSTNAMESTRSIZE, h ? h : lh,
			strlen(h ? h : lh));
	memrpl(e->self->realname, USERNAMESTRSIZE, n ? n : p->pw_name,
			strlen(n ? n : p->pw_name));
	memrpl(e->self->username, USERNAMESTRSIZE, n ? n : p->pw_name,
			strlen(n ? n : p->pw_name));
	if (!e->nick[0])
		return (0);
	cbuffer_putcmd(&e->self->buf_write, "NICK %s\x0D\x0A", e->nick);
	memrpl(e->self->nickname, NICKNAMESTRSIZE, e->nick, strlen(e->nick));
	return (0);
}

int			c2s_connect(t_env *e, t_token *tokens)
{
	if (e->sock != -1)
		return (logerror("Already connected"));
	if ((c2s_connect_check_command(e, tokens)) < 0)
		return (-1);
	if (do_c2s_connect(e, NULL, NULL, tokens[1].addr) < 0)
		return (-1);
	return (IRC_C2S_CONNECT);
}
