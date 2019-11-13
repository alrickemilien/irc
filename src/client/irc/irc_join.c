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

static int irc_join_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    (void)cs;
    (void)e;

    if (!tokens[1].addr || tokens[2].addr)
        return logerror("ERR_NEEDMOREPARAMS");

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    if (strpbrk(channel, "\x07\x2C"))
            return logerror("ERR_NOSUCHCHANNEL");
    else if (channel_len - 1 > CHANNELSTRSIZE)
        return logerror("ERR_NOSUCHCHANNEL");
    else if ((channel[0] != '#' && channel[0] != '&') ||
             !is_valid_chan(channel))
        return logerror("ERR_NOSUCHCHANNEL");
    else if (channel_len < 1)
        return logerror("ERR_NOSUCHCHANNEL");
    else
        return (0);
    return (-1);
}

int irc_join(t_env *e, int cs, t_token *tokens)
{
    if ((irc_join_check_command(e, cs, tokens)) != 0)
        return (-1);

    strncat(e->fds[cs].buf_write, tokens[1].addr, tokens[1].len);

    if (!e->fds[cs].channelname[0])
        loginfo("You joined %s", tokens[1].addr);
    else
        loginfo("You leaved %s for %s", e->fds[cs].channel, tokens[1].addr);
    
    memset(e->fds[cs].channelname, 0, CHANNELSTRSIZE + 1);
    memcpy(e->fds[cs].channelname, tokens[1].addr, tokens[1].len);

    return (IRC_JOIN);
}
