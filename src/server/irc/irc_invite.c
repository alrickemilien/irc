#include <server/irc.h>

static int irc_invite_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *cs_channel;

    // Check all parameters are available
    if (!tokens[1].addr || !tokens[2].addr)
        return (irc_err(e, cs, ERR_NEEDMOREPARAMS, NULL));

    cs_channel = e->channels[e->fds[cs].channel].channel;

    // Check valid nickname
    if (!tokens[1].len)
        return (irc_err(e, cs, ERR_NOSUCHNICK, NULL));

    // Check valid channel and channel match user one
    if (strncmp(cs_channel, tokens[2].addr, tokens[2].len) != 0 ||
        cs_channel[tokens[2].len] != 0)
        return (irc_err(e, cs, ERR_NOSUCHCHANNEL, cs_channel));

    // Wheck channel operator
    else if (e->channels[e->fds[cs].channel].chop != cs)
        return (irc_err(e, cs, ERR_CHANOPRIVSNEEDED, cs_channel));
    return (0);
}

int irc_invite(t_env *e, int cs, t_token *tokens)
{
    size_t      i;
    const char *cs_channel;
    const char *i_channel;

    if ((irc_invite_check_command(e, cs, tokens)) != 0)
        return (-1);

    cs_channel = e->channels[e->fds[cs].channel].channel;

    i = 0;
    while (i <= e->max)
    {
        if (strncmp(e->fds[i].nickname, tokens[1].addr, tokens[1].len) == 0)
        {
            i_channel = e->channels[e->fds[i].channel].channel;

            // When user alreayd into the channel
            if (strncmp(i_channel, tokens[2].addr, tokens[2].len) == 0 &&
                i_channel[tokens[2].len] == 0)
            {
                irc_reply(e, cs, ERR_USERONCHANNEL, e->fds[i].nickname,
                          i_channel);
            }
            else if (e->fds[i].away)
            {
                irc_reply(e, cs, RPL_AWAY, e->fds[i].awaymessage);
            }
            else
            {
                cbuffer_putcmd(&e->fds[i].buf_write, ":%s INVITE %s %s\x0D\x0A",
                               e->fds[cs].nickname, e->fds[i].nickname,
                               cs_channel);

                irc_reply(e, cs, RPL_INVITING, e->fds[i].nickname, cs_channel,
                          e->fds[i].nickname);
            }

            return (IRC_INVITE);
        }
        i++;
    }

    irc_reply(e, cs, ERR_NOSUCHNICK, e->fds[i].nickname);

    return (IRC_INVITE);
}
