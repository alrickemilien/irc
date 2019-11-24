#include <client/irc.h>
#include <ctype.h>

/*
** the only restriction on a
** channel name is that it may not contain any spaces (' '), a control G
** (^G or ASCII 7), or a comma (',' which is used as a list item
** separator by the protocol)
*/

static bool is_valid_chan(const char *channel)
{
    size_t i;

    i = 1;
    while (channel[i] && channel[i] != '\x0D')
    {
        if (channel[i] == '\x07' || channel[i] == '\x20' ||
            channel[i] == '\x2C')
            return (false);
        i++;
    }
    return (true);
}

static int c2s_leave_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr || tokens[2].addr)
        return logerror("c2s_leave_check_command::ERR_NEEDMOREPARAMS\n");

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    if (strpbrk(channel, "\x07\x2C"))
        return logerror("c2s_leave_check_command::ERR_NOSUCHCHANNEL\n");
    else if (channel_len - 1 > CHANNELSTRSIZE)
        return logerror("c2s_leave_check_command::ERR_NOSUCHCHANNEL\n");
    else if ((channel[0] != '#' && channel[0] != '&') ||
             !is_valid_chan(channel))
        return logerror("c2s_leave_check_command::ERR_NOSUCHCHANNEL\n");
    else if (channel_len < 1)
        return logerror("c2s_leave_check_command::ERR_NOSUCHCHANNEL\n");
    else
        return (0);
    return (-1);
}

int _c2s_leave(t_fd *fd, const char *channel_name, size_t channel_name_len)
{
    return (cbuffer_putcmd(&fd->buf_write, "PART %*s\x0D\x0A", channel_name_len,
                           channel_name));
}

int c2s_leave(t_env *e, int cs, t_token *tokens)
{
    if (e->sock == -1)
        return logerror("%s\n",
                        "You need to be logged in before any command. Use "
                        "/connect [server] ?[port]");

    if ((c2s_leave_check_command(e, cs, tokens)) != 0)
        return (-1);

    _c2s_leave(&e->fds[e->sock], tokens[1].addr, tokens[1].len);

    return (IRC_LEAVE);
}
