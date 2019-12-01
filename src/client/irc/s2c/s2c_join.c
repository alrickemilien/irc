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
        return (irc_error(e, ERR_NEEDMOREPARAMS));

    channel = tokens[2].addr;
    channel_len = tokens[2].len;

    if (strpbrk(channel, "\x07\x2C"))
        return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
    else if (channel_len - 1 > CHANNELSTRSIZE)
        return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
    else if ((channel[0] != '#' && channel[0] != '&') ||
             !is_valid_chan_name(channel))
        return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
    else if (channel_len < 1)
        return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
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

    logdebug("s2c_join::cli->nickname:: %s", cli->nickname);
    logdebug("s2c_join::user:: %s", user);
    logdebug("s2c_join::expected:: %s", expected);

    return (strncmp(expected, user, len) == 0);
}

int s2c_join(t_env *e, int cs, t_token *tokens)
{
    char log[512];

    logdebug("s2c_join:: %s", tokens[0].addr);

    if (s2c_join_check_command(e, cs, tokens) < 0)
        return (-1);

    memset(log, 0, sizeof(log));
    sprintf(log, "%.*s joined the channel.", (int)tokens[0].len, tokens[0].addr);

    if (s2c_join_is_me(e, tokens[0].addr, tokens[0].len))
    {
        memrpl(e->fds[e->sock].channelname, CHANNELSTRSIZE, tokens[2].addr,
               tokens[2].len);

        loginfo(log);

        if (e->options.gui)
            ui_join(e->ui, e->fds[e->sock].channelname);
        return (IRC_S2C_JOIN);
    }

    if (e->options.gui)
        ui_new_message(e->ui, log, UI_CHAT_MSG);

    return (IRC_S2C_JOIN);
}
