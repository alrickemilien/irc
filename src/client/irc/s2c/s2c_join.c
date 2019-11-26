#include <client/irc.h>
#include <client/ui/panel.h>
#include <ctype.h>

static int s2c_join_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    (void)cs;
    (void)e;

    if (!tokens[0].addr || !tokens[1].addr || !tokens[2].addr)
        return logerror("s2c_join_check_command::ERR_NEEDMOREPARAMS\n");

    channel = tokens[2].addr;
    channel_len = tokens[2].len;

    if (strpbrk(channel, "\x07\x2C"))
        return logerror("s2c_join_check_command::ERR_NOSUCHCHANNEL\n");
    else if (channel_len - 1 > CHANNELSTRSIZE)
        return logerror("s2c_join_check_command::ERR_NOSUCHCHANNEL\n");
    else if ((channel[0] != '#' && channel[0] != '&') ||
             !is_valid_chan_name(channel))
        return logerror("s2c_join_check_command::ERR_NOSUCHCHANNEL\n");
    else if (channel_len < 1)
        return logerror("s2c_join_check_command::ERR_NOSUCHCHANNEL\n");
    return (0);
}

/*
** Return true when the JOIN user in the message is the current client
*/

static bool s2c_join_is_me(t_env *e, const char *user, size_t len)
{
    char  expected[512];
    t_fd *cli;

    cli = &e->fds[e->sock];

    memset(expected, 0, sizeof(expected));

    sprintf(expected, "%s!%s@%s", cli->nickname, cli->username, cli->host);

    logdebug("s2c_join::cli->nickname:: %s\n", cli->nickname);
    logdebug("s2c_join::user:: %s\n", user);
    logdebug("s2c_join::expected:: %s\n", expected);

    return (strncmp(expected, user, len) == 0);
}

int s2c_join(t_env *e, int cs, t_token *tokens)
{
    logdebug("s2c_join:: %s\n", tokens[0].addr);

    if (s2c_join_check_command(e, cs, tokens) < 0)
        return (-1);

    if (s2c_join_is_me(e, tokens[0].addr, tokens[0].len))
    {
        memrpl(e->fds[e->sock].channelname, CHANNELSTRSIZE, tokens[2].addr,
               tokens[2].len);

        loginfo("s2c_privmsg:: You joined %s\n", e->fds[e->sock].channelname);

        if (e->options.gui)
            set_channel_name(e->fds[e->sock].channelname);
    }

    return (IRC_S2C_JOIN);
}
