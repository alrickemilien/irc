#include <server/irc.h>

static const t_irc_reply g_replys[] = {
    {RPL_WELCOME, "001",
     ":Welcome to the Internet Relay Network %s!%s@%s"},
    {RPL_AWAY, "301", "%s :%s"},
    {RPL_UNAWAY, "305", ":You are no longer marked as being away"},
    {RPL_NOWAWAY, "306", ":You have been marked as being away"},
    {RPL_NAMREPLY, "353", "%s :%s"},
    {RPL_ENDOFNAMES, "366", "%s :End of /NAMES list"},
    {ERR_NOSUCHNICK, "401", "%s :No such nick/channel"},
    {ERR_NOSUCHSERVER, "402", "%s :No such server"},
    {ERR_NOSUCHCHANNEL, "403", "%s :No such channel"},
    {ERR_NICKNAMEINUSE, "433", "%s :Nickname is already in use"},
    {ERR_ERRONEUSNICKNAME, "432", "%s :Erroneus nickname"},
    {ERR_NONICKNAMEGIVEN, "431", ":No nickname given"},
    {ERR_NEEDMOREPARAMS, "461", "%s :Not enough parameters"},
    {ERR_ALREADYREGISTRED, "462", ":You may not reregister"},
    {ERR_NOTEXTTOSEND, "412", ":No text to send"},
    {ERR_NOTONCHANNEL, "442", "%s :You're not on that channel"},
    {RPL_WHOISUSER, "311", "%s %s %s * :%s"},
    {RPL_ENDOFWHOIS, "318", "%s :End of /WHOIS list"},
    {RPL_WHOISCHANNELS, "319", "%s :%s"},
    {RPL_WHOREPLY, "352", "%s %s %s %s %s %s"},
    {RPL_ENDOFWHO, "315", "%s :End of /WHO list"},
    {RPL_TOPIC, "332", "%s :%s"},
    {RPL_LISTSTART, "321", "Channel :Users  Name"},
    {RPL_LIST, "322", "%s :%s" /*"<channel> <# visible> :<topic>"*/},
    {RPL_LISTEND, "323", ":End of /LIST"},
    {RPL_TIME, "391", "%s: %s"},
    {RPL_VERSION, "351", "%s.%s %s :%s"},
    {ERR_USERONCHANNEL, "443", "%s %s :is already on channel"},
    {RPL_INVITING, "341", "%s %s"},
    {RPL_NOTOPIC, "331", "%s :No topic is set"},
    {ERR_CHANOPRIVSNEEDED, "482",
     "%s :You're not channel operator"},
};

int irc_reply(t_env *e, int cs, int code, ...)
{
    size_t  i;
    char    msg[512];
    va_list ap;

    i = 0;
    while (i < sizeof(g_replys) / sizeof(t_irc_reply))
    {
        if (g_replys[i].code == code)
        {
            sprintf(msg, "%s %s\x0D\x0A", g_replys[i].name, g_replys[i].fmt);
            va_start(ap, code);
            cbuffer_putvcmd(&e->fds[cs].buf_write, msg, ap);
            va_end(ap);
            return (0);
        }
        i++;
    }

    return (-1);
}

int irc_reply_all_in_channel(t_env *e, int cs, int code, ...)
{
    size_t  i;
    char    msg[512];
    va_list ap;

    i = 0;
    while (i < sizeof(g_replys) / sizeof(t_irc_reply))
    {
        if (g_replys[i].code == code)
        {
            sprintf(msg, "%s %s\x0D\x0A", g_replys[i].name, g_replys[i].fmt);
            va_start(ap, code);
            break;
        }
        i++;
    }

    if (i == sizeof(g_replys) / sizeof(t_irc_reply))
        return (-1);

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT &&
            e->fds[i].channel == e->fds[cs].channel)
            cbuffer_putvcmd(&e->fds[cs].buf_write, msg, ap);
        i++;
    }

    va_end(ap);

    return (-1);
}

int irc_err(t_env *e, int cs, int code, ...)
{
    size_t  i;
    char    msg[512];
    va_list ap;

    i = 0;
    while (i < sizeof(g_replys) / sizeof(t_irc_reply))
    {
        if (g_replys[i].code == code)
        {
            sprintf(msg, "%s %s\x0D\x0A", g_replys[i].name, g_replys[i].fmt);
            va_start(ap, code);
            cbuffer_putvcmd(&e->fds[cs].buf_write, msg, ap);
            va_end(ap);
            return (-1);
        }
        i++;
    }

    return (-1);
}
