#include <client/irc.h>
#include <ctype.h>
#include <netdb.h>
#include <pwd.h>
#include <sys/types.h>

static int c2s_connect_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t host_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr)
        return (logerror("c2s_connect_check_command:: NO HOST GIVEN\n"));

    host_len = tokens[1].len;

    if (host_len > NI_MAXHOST || !host_len)
        return (logerror("c2s_connect_check_command:: INVALID HOSTNAME\n"));

    memset(e->options.host, 0, sizeof(e->options.host));
    memcpy(e->options.host, tokens[1].addr, tokens[1].len);

    if (!tokens[2].addr)
        return (0);

    if (ato32(tokens[2].addr, (uint32_t *)&e->options.port) != 0 ||
        e->options.port < 1000 || e->options.port > 99999)
        return (
            logerror("c2s_connect_check_command:: port must be a value between "
                     "1000 an 99999: '%s'.\n",
                     tokens[2].addr));

    return (0);
}

int _c2s_connect(t_env *     e,
                 const char *name,
                 const char *hostname,
                 const char *servername)
{
    char           concat[512];
    char           local_hostname[NI_MAXHOST + 1];
    int            cs;
    t_fd *         fd;
    struct passwd *p;

    if ((p = getpwuid(getuid())) == NULL)
        return (-1);

    if (gethostname(local_hostname, sizeof(local_hostname)) == -1)
        return (-1);

    if (e->ipv6 == 1)
        client_ipv6(e);
    else
        client_ipv4(e);

    if (e->sock == -1)
        return (logerrno("c2s_connect::"));

    cs = e->sock;
    fd = &e->fds[cs];

    sprintf(concat, "USER %s %s %s %s\x0D\x0A", name ? name : p->pw_name,
            hostname ? hostname : local_hostname, servername,
            name ? name : p->pw_name);

    memset(concat, 0, sizeof(concat));

    cbuffer_put(&fd->buf_write, (uint8_t *)concat, strlen(concat));

    loginfo("Connecting to %s\n", servername);

    memset(fd->nickname, 0, NICKNAMESTRSIZE - 1);
    memcpy(fd->nickname, servername, strlen(servername));

    memset(fd->realname, 0, USERNAMESTRSIZE - 1);
    memcpy(fd->realname, name ? name : p->pw_name, strlen(name ? name : p->pw_name));

    memset(fd->username, 0, USERNAMESTRSIZE - 1);
    memcpy(fd->username, name ? name : p->pw_name, strlen(name ? name : p->pw_name));

    return (0);
}

int c2s_connect(t_env *e, int cs, t_token *tokens)
{
    if (e->sock != -1)
        return logerror("Already connected\n");

    if ((c2s_connect_check_command(e, cs, tokens)) < 0)
        return (-1);

    // Command: USER
    // Parameters: <username> <hostname> <servername> <realname>
    _c2s_connect(e, NULL, NULL, tokens[1].addr);

    return (IRC_CONNECT);
}
