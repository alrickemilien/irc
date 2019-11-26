#include <ctype.h>
#include <server/irc.h>

/*
** Command: USER
** Parameters: <username> <hostname> <servername> <realname>
**
** Between servers USER must to be prefixed with client's NICKname.
** Note that hostname and servername are normally ignored by the IRC
** server when the USER command comes from a directly connected client,
** but they are used in server to server
** communication.
*/

static int irc_user_check_command(t_env *e, int cs, const t_token *tokens)
{
    char *username;
    // char *hostname;
    // char *servername;
    // char *realname;

    size_t username_len;
    // size_t hostname_len;
    // size_t servername_len;
    // size_t realname_len;

    size_t i;

    if (!tokens[1].addr || !tokens[2].addr || !tokens[3].addr ||
        !tokens[4].addr)
    {
        irc_reply(e, cs, ERR_NEEDMOREPARAMS, "USER");
        return (-1);
    }

    username = tokens[1].addr;
    username_len = tokens[1].len;

    // ignored here
    // hostname = tokens[2].addr;
    // hostname_len = tokens[2].len;

    // servername = tokens[3].addr;
    // servername_len = tokens[3].len;

    // realname = tokens[4].addr;
    // realname_len = tokens[4].len;

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT &&
            strncmp(e->fds[i].username, username, username_len) == 0)
        {
            irc_reply(e, cs, ERR_ALREADYREGISTRED, NULL);
            return (-1);
        }
        i++;
    }

    return (0);
}

void irc_user_join_default_channel(t_env *e, int cs)
{
    char concat[512];

    e->channels[e->fds[cs].channel].clients++;

    memset(concat, 0, sizeof(concat));

    sprintf(concat, "%s!%s@%s JOIN %s\x0D\x0A", e->fds[cs].nickname,
            e->fds[cs].nickname, e->fds[cs].host,
            e->channels[e->fds[cs].channel].channel);

    logdebug("irc_user_join_default_channel:: %s\n", concat);
    cbuffer_putstr(&e->fds[cs].buf_write, concat);
}

int irc_user(t_env *e, int cs, t_token *tokens)
{
    if ((irc_user_check_command(e, cs, tokens)) < 0)
        return (-1);

    // Set username
    memrpl(e->fds[cs].username, USERNAMESTRSIZE, tokens[1].addr,
           tokens[1].len < USERNAMESTRSIZE ? tokens[1].len : USERNAMESTRSIZE);

    // Set hostname
    memrpl(e->fds[cs].host, HOSTNAMESTRSIZE, tokens[1].addr,
           tokens[1].len < HOSTNAMESTRSIZE ? tokens[1].len : HOSTNAMESTRSIZE);

    // Set realname
    memrpl(e->fds[cs].realname, USERNAMESTRSIZE,
           tokens[4].addr[0] == ':' ? tokens[4].addr + 1 : tokens[4].addr,
           tokens[4].addr[0] == ':' ? tokens[4].len - 1 : tokens[4].len);

    // When nickname is not set
    if (e->fds[cs].nickname[0] == 0)
    {
        logdebug(
            "USER command received for client #%ld without nickname, do not "
            "set as registered now\n",
            cs);
        return (IRC_USER);
    }

    e->fds[cs].registered = 1;

    irc_reply(e, cs, RPL_WELCOME, e->fds[cs].username, e->fds[cs].host,
              e->fds[cs].realname);

    logdebug("irc_user:: USER %s@%s %s join the server\n", e->fds[cs].username,
             e->fds[cs].host, e->fds[cs].realname);

    irc_user_join_default_channel(e, cs);

    return (IRC_USER);
}
