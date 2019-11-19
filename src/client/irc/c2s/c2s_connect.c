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

int c2s_connect(t_env *e, int cs, t_token *tokens)
{
    struct passwd *p;
    char           hostname[NI_MAXHOST + 1];
    char           concat[512];

    if (e->sock != -1)
        return logerror("Already connected\n");

    if ((c2s_connect_check_command(e, cs, tokens)) < 0)
        return (-1);

    if (e->ipv6 == 1)
        client_ipv6(e);
    else
        client_ipv4(e);

    if (e->sock == -1)
        return (logerrno("c2s_connect::"));

    cs = e->sock;

    if ((p = getpwuid(getuid())) == NULL)
        return (-1);

    if (gethostname(hostname, sizeof(hostname)) == -1)
        return (-1);

    // Command: USER
    // Parameters: <username> <hostname> <servername> <realname>

    memset(concat, 0, sizeof(concat));
    strcat(concat, "USER ");
    strcat(concat, p->pw_name);
    strcat(concat, " ");
    strcat(concat, hostname);
    strcat(concat, " ");
    strncat(concat, tokens[1].addr, tokens[1].len);
    strcat(concat, " ");
    strcat(concat, p->pw_name);

    strcat(concat, "\x0D\x0A");
    cbuffer_put(&e->fds[cs].buf_write, (uint8_t *)concat, strlen(concat));

    loginfo("Connecting to %s\n", tokens[1].addr);

    memset(e->fds[cs].nickname, 0, NICKNAMESTRSIZE);
    memcpy(e->fds[cs].nickname, tokens[1].addr, tokens[1].len);

    memset(e->fds[cs].realname, 0, USERNAMESTRSIZE);
    memcpy(e->fds[cs].realname, p->pw_name, strlen(p->pw_name));

    memset(e->fds[cs].username, 0, USERNAMESTRSIZE);
    memcpy(e->fds[cs].username, p->pw_name, strlen(p->pw_name));

    return (IRC_NICK);
}
