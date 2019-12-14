#include <client/irc.h>
#include <client/ui/panel.h>

static int s2c_join_check_command(t_env *e, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    if (!tokens[0].addr || !tokens[1].addr || !tokens[2].addr)
        return (irc_error(e, ERR_NEEDMOREPARAMS));

    channel = tokens[2].addr;
    channel_len = tokens[2].len;

    if (channel_len - 1 > CHANNELSTRSIZE)
        return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
    else if (!is_valid_chan_name(channel, channel_len))
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

    memset(expected, 0, sizeof(expected));

    sprintf(expected, "%s!%s@%s", e->self->nickname, e->self->username, e->self->host);

    logdebug("s2c_join::cli->nickname:: %s", e->self->nickname);
    logdebug("s2c_join::user:: %s", user);
    logdebug("s2c_join::expected:: %s", expected);

    return (strncmp(expected, user, len) == 0);
}

int s2c_join(t_env *e, t_token *tokens)
{
    char msg[512];

    if (s2c_join_check_command(e, tokens) < 0)
        return (-1);

    memset(msg, 0, sizeof(msg));
    sprintf(msg, "%.*s joined the channel.", (int)tokens[0].len, tokens[0].addr);

    if (s2c_join_is_me(e, tokens[0].addr, tokens[0].len))
    {
        memrpl(e->self->channelname, CHANNELSTRSIZE, tokens[2].addr,
               tokens[2].len);

        loginfo(msg);

        if (e->options.gui)
            ui_join(e->ui, e->self->channelname);
        return (IRC_S2C_JOIN);
    }

    if (e->options.gui)
        ui_new_message(e->ui, msg, UI_INFO_MSG);

    return (IRC_S2C_JOIN);
}
