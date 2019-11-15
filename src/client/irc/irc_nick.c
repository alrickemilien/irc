#include <ctype.h>
#include <client/irc.h>

static int irc_nick_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t      nick_len;

    (void)cs;
    (void)e;
    
    if (!tokens[1].addr || tokens[2].addr)
        return (logerror("ERR_NONICKNAMEGIVEN"));

    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len)
        return (logerror("ERR_ERRONEUSNICKNAME"));

    return (0);
}

int irc_nick(t_env *e, int cs, t_token *tokens)
{
    if ((irc_nick_check_command(e, cs, tokens)) < 0)
        return (-1);

    loginfo("You changed nickname to %s\n", tokens[1].addr);

    memset(e->fds[cs].nickname, 0, NICKNAMESTRSIZE);
    memcpy(e->fds[cs].nickname, tokens[1].addr, tokens[1].len);

    return (IRC_NICK);
}
