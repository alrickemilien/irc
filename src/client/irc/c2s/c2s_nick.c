#include <ctype.h>
#include <client/irc.h>

static int c2s_nick_check_command(t_env *e, int cs, const t_token *tokens)
{
    size_t nick_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr || tokens[2].addr)
        return (logerror("ERR_NONICKNAMEGIVEN\n"));

    nick_len = tokens[1].len;

    if (nick_len > 9 || !nick_len)
        return (logerror("ERR_ERRONEUSNICKNAME\n"));

    return (0);
}

int _c2s_nick(t_fd *fd, const char *nick, size_t nick_length)
{
    cbuffer_putstr(&fd->buf_write, "NICK ");
    cbuffer_put(&fd->buf_write, (const uint8_t *)nick, nick_length);
    cbuffer_putstr(&fd->buf_write, "\x0D\x0A");

    loginfo("You changed nickname to %s\n", nick);

    memset(fd->nickname, 0, NICKNAMESTRSIZE);
    memcpy(fd->nickname, nick, nick_length);

    return (0);
}

int c2s_nick(t_env *e, int cs, t_token *tokens)
{
    if (e->sock == -1)
        return logerror(
            "%s\n",
            "You nee to be logged in before any command. Use /connect [server] "
            "?[port]");

    if ((c2s_nick_check_command(e, cs, tokens)) < 0)
        return (-1);

    _c2s_nick(&e->fds[cs], tokens[1].addr, tokens[1].len);

    return (IRC_NICK);
}
