#include <ctype.h>
#include "server/irc.h"

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

    // size_t username_len;
    // size_t hostname_len;
    // size_t servername_len;
    // size_t realname_len;

    size_t i;

    if (!tokens[1].addr || !tokens[2].addr || !tokens[3].addr ||
        !tokens[4].addr)
    {
        irc_reply(e, cs, ERR_NEEDMOREPARAMS, tokens[0].addr);
        return (-1);
    }

    username = tokens[1].addr;
    // username_len = tokens[1].len;

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
        if (i != (size_t)cs && e->fds[i].type == FD_CLIENT &&
            strcmp(e->fds[i].username, username) == 0)
        {
            irc_reply(e, cs, ERR_ALREADYREGISTRED, NULL);
            return (-1);
        }
        i++;
    }

    return (0);
}

void irc_user(t_env *e, int cs, t_token *tokens)
{
    char concat[512];

    if ((irc_user_check_command(e, cs, tokens)) != 0)
        return;

    // Set username
    memset(e->fds[cs].username, 0, USERNAMESTRSIZE);
    memcpy(e->fds[cs].username, tokens[1].addr,
           tokens[1].len < USERNAMESTRSIZE ? tokens[1].len : USERNAMESTRSIZE);

    // Set hostname
    memset(e->fds[cs].hostname, 0, HOSTNAMESTRSIZE);
    memcpy(e->fds[cs].hostname, tokens[2].addr,
           tokens[2].len < HOSTNAMESTRSIZE ? tokens[2].len : HOSTNAMESTRSIZE);

    // Set realname
    memset(e->fds[cs].realname, 0, USERNAMESTRSIZE);
    memcpy(e->fds[cs].realname, tokens[4].addr,
           tokens[4].len < USERNAMESTRSIZE ? tokens[4].len : USERNAMESTRSIZE);

    e->fds[cs].registered = 1;

    memset(concat, 0, sizeof(concat));
    time2iso(e->isotime);
    sprintf(concat, "%s from %s registered with real name %s",
            e->fds[cs].username, e->fds[cs].hostname, e->fds[cs].realname);
    broadcast(e, concat, IRC_INFO, cs);
}
