#include <client/irc.h>
#include <ctype.h>
#include <netdb.h>
#include <pwd.h>
#include <sys/types.h>

static int c2s_connect_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t nick_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr || tokens[2].addr)
        return (logerror("ERR_NONICKNAMEGIVEN"));

    nick_len = tokens[1].len;

    if (nick_len > NI_MAXHOST || !nick_len)
        return (logerror("ERR_ERRONEUSNICKNAME"));

    return (0);
}

int c2s_connect(t_env *e, int cs, t_token *tokens)
{
    struct passwd *p;
    char           hostname[NI_MAXHOST + 1];
    char           concat[512];
    int            ret;

    if ((c2s_connect_check_command(e, cs, tokens)) < 0)
        return (-1);

    if ((p = getpwuid(getuid())) == NULL)
        return (-1);

    if (gethostname(hostname, sizeof(hostname)) == -1)
        return (-1);

    // Command: USER
    // Parameters: <username> <hostname> <servername> <realname>

    printf("User name: %s\n", p->pw_name);
    printf("Host name: %s\n", hostname);

    memset(concat, 0, sizeof(concat));
    ret = sprintf(concat, "USER %s %s %s %s\x0A\x0D", p->pw_name, hostname,
                  tokens[1].addr, p->pw_name);

    cbuffer_put(&e->fds[cs].buf_write, (uint8_t*)concat, ret);

    loginfo("Connecting to %s\n", tokens[1].addr);

    memset(e->fds[cs].nickname, 0, NICKNAMESTRSIZE);
    memcpy(e->fds[cs].nickname, tokens[1].addr, tokens[1].len);

    memset(e->fds[cs].realname, 0, USERNAMESTRSIZE);
    memcpy(e->fds[cs].realname, p->pw_name, strlen(p->pw_name));

    memset(e->fds[cs].username, 0, USERNAMESTRSIZE);
    memcpy(e->fds[cs].username, p->pw_name, strlen(p->pw_name));

    return (IRC_NICK);
}
