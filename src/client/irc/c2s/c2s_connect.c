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
		return (logerror("c2s_connect:: port be 1000-99999: '%s'.\n",
					tokens[2].addr));
	return (0);
}

int			do_c2s_connect(t_env *e,
		const char *name,
		const char *hostname,
		const char *servername)
{
	char			local_hostname[NI_MAXHOST + 1];
	int				cs;
	t_fd			*fd;
	struct passwd	*p;

	if ((p = getpwuid(getuid())) == NULL)
		return (logerrno("do_c2s_connect::getpwuid"));
	if (gethostname(local_hostname, sizeof(local_hostname)) == -1)
		return (logerrno("do_c2s_connect::gethostname"));
	if (e->ipv6 == 1)
		client_ipv6(e);
	else
		client_ipv4(e);
	if (e->sock == -1)
		return (-1);
	cs = e->sock;
	fd = &e->fds[cs];
	e->self = &e->fds[cs];
	cbuffer_putcmd(&fd->buf_write, "USER %s %s %s %s\x0D\x0A",
			name ? name : p->pw_name,
			hostname ? hostname : local_hostname, servername,
			name ? name : p->pw_name);
	loginfo("Connecting to %s", e->options.host);
	memrpl(fd->host, HOSTNAMESTRSIZE, hostname ? hostname : local_hostname,
			strlen(hostname ? hostname : local_hostname));
	memrpl(fd->realname, USERNAMESTRSIZE, name ? name : p->pw_name,
			strlen(name ? name : p->pw_name));
	memrpl(fd->username, USERNAMESTRSIZE, name ? name : p->pw_name,
			strlen(name ? name : p->pw_name));
	if (e->nick[0])
	{
		cbuffer_putcmd(&fd->buf_write, "NICK %s\x0D\x0A", e->nick);
		memrpl(fd->nickname, NICKNAMESTRSIZE, e->nick, strlen(e->nick));
	}
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
