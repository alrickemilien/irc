#include <ctype.h>
#include <server/irc.h>

static int irc_nick_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *nick;
    size_t      nick_len;
    size_t      i;

    if (!tokens[1].addr || tokens[2].addr)
    {
        irc_reply(e, cs, ERR_NONICKNAMEGIVEN, NULL);
        return (-1);
    }

    nick = tokens[1].addr;
    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len || nick[0] == '#' || nick[0] == '&')
    {
        irc_reply(e, cs, ERR_ERRONEUSNICKNAME, nick);
        return (-1);
    }

    i = 0;
    while (i <= e->max)
    {
        if (i != (size_t)cs && e->fds[i].type == FD_CLIENT &&
            strcmp(e->fds[i].nickname, nick) == 0)
        {
            irc_reply(e, cs, ERR_NICKNAMEINUSE, nick);
            return (-1);
        }

        i++;
    }

    return (0);
}

int irc_nick(t_env *e, int cs, t_token *tokens)
{
    char concat[512];

    if ((irc_nick_check_command(e, cs, tokens)) != 0)
        return (-1);

    logdebug("irc_nick:: %s\n", tokens[0].addr);

    memset(concat, 0, sizeof(concat));

    if (e->fds[cs].registered)
    {
        logdebug("irc_nick::registered\n");

        sprintf(concat, "%s changed nickname to %s", e->fds[cs].nickname,
                tokens[1].addr);
        broadcast(e, concat, IRC_NOTICE, cs);

        logdebug("%s\n", concat);

        return (IRC_NICK);
    }

    logdebug("irc_nick::unregistered\n");

    memrpl(e->fds[cs].nickname, NICKNAMESTRSIZE, tokens[1].addr, tokens[1].len);

    // When nickname is not set
    if (e->fds[cs].username[0] == 0)
    {
        logdebug("irc_nick::When nickname is not set\n");

        return (IRC_NICK);
    }

    e->fds[cs].registered = 1;

    irc_reply(e, cs, RPL_WELCOME, e->fds[cs].username, e->fds[cs].host,
              e->fds[cs].realname);

    logdebug("irc_user:: USER %s@%s %s join the server\n", e->fds[cs].username,
             e->fds[cs].host, e->fds[cs].realname);

    irc_user_join_default_channel(e, cs);

    return (IRC_NICK);
}
