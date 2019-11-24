#include <ctype.h>
#include <client/irc.h>

static int c2s_nick_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t nick_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr || tokens[2].addr)
        return (logerror("c2s_nick_check_command::ERR_NONICKNAMEGIVEN\n"));

    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len)
        return (logerror("c2s_nick_check_command::ERR_ERRONEUSNICKNAME\n"));

    return (0);
}

int _c2s_nick(t_env *e, const char *nick, size_t nick_length)
{
    // Set only local nickname
    if (e->sock == -1)
    {
        memset(e->nick, 0, NICKNAMESTRSIZE);
        memcpy(e->nick, nick, nick_length);
        return (0);
    }

    cbuffer_putstr(&e->fds[e->sock].buf_write, "NICK ");
    cbuffer_put(&e->fds[e->sock].buf_write, (const uint8_t *)nick, nick_length);
    cbuffer_putstr(&e->fds[e->sock].buf_write, "\x0D\x0A");

    loginfo("You changed nickname to %s\n", nick);

    memset(e->fds[e->sock].nickname, 0, NICKNAMESTRSIZE);
    memcpy(e->fds[e->sock].nickname, nick, nick_length);

    return (0);
}

int c2s_nick(t_env *e, int cs, t_token *tokens)
{
    if ((c2s_nick_check_command(e, cs, tokens)) < 0)
        return (-1);

    _c2s_nick(e, tokens[1].addr, tokens[1].len);

    return (IRC_NICK);
}
