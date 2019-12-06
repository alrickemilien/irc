#include <server/irc.h>

static const t_irc_reply g_replys[] = {
    {RPL_WELCOME, "RPL_WELCOME",
     ":Welcome to the Internet Relay Network %s!%s@%s"},
    {RPL_AWAY, "RPL_AWAY", "%s :%s"},
    {RPL_UNAWAY, "RPL_UNAWAY", ":You are no longer marked as being away"},
    {RPL_NOWAWAY, "RPL_NOWAWAY", ":You have been marked as being away"},
    {RPL_NAMREPLY, "RPL_NAMREPLY", "%s :%s"},
    {RPL_ENDOFNAMES, "RPL_ENDOFNAMES", "%s :End of /NAMES list"},
    {ERR_NOSUCHNICK, "ERR_NOSUCHNICK", "%s :No such nick/channel"},
    {ERR_NOSUCHSERVER, "ERR_NOSUCHSERVER", "%s :No such server"},
    {ERR_NOSUCHCHANNEL, "ERR_NOSUCHCHANNEL", "%s :No such channel"},
    {ERR_NICKNAMEINUSE, "ERR_NICKNAMEINUSE", "%s :Nickname is already in use"},
    {ERR_ERRONEUSNICKNAME, "ERR_ERRONEUSNICKNAME", "%s :Erroneus nickname"},
    {ERR_NONICKNAMEGIVEN, "ERR_NONICKNAMEGIVEN", ":No nickname given"},
    {ERR_NEEDMOREPARAMS, "ERR_NEEDMOREPARAMS", "%s :Not enough parameters"},
    {ERR_ALREADYREGISTRED, "ERR_ALREADYREGISTRED", ":You may not reregister"},
    {ERR_NOTEXTTOSEND, "ERR_NOTEXTTOSEND", ":No text to send"},
    {ERR_NOTONCHANNEL, "ERR_NOTONCHANNEL", "%s :You're not on that channel"},
    {RPL_WHOISUSER, "RPL_WHOISUSER", "%s %s %s * :%s"},
    {RPL_ENDOFWHOIS, "RPL_ENDOFWHOIS", "%s :End of /WHOIS list"},
    {RPL_WHOISCHANNELS, "RPL_WHOISCHANNELS", "%s :%s"},
    {RPL_WHOREPLY, "RPL_WHOREPLY", "%s %s %s %s %s %s"},
    {RPL_ENDOFWHO, "RPL_ENDOFWHO", "%s :End of /WHO list"},
    {RPL_TOPIC, "RPL_TOPIC", "%s :%s"},
    {RPL_LISTSTART, "RPL_LISTSTART", "Channel :Users  Name"},
    {RPL_LIST, "RPL_LIST", "%s" /*"<channel> <# visible> :<topic>"*/},
    {RPL_LISTEND, "RPL_LISTEND", ":End of /LIST"},
    {RPL_TIME, "RPL_TIME", "%s: %s"},
    {RPL_VERSION, "RPL_VERSION", "%s.%s %s :%s"},
    {ERR_USERONCHANNEL, "ERR_USERONCHANNEL", "%s %s :is already on channel"},
    {RPL_INVITING, "RPL_INVITING", "%s %s"},
    {RPL_NOTOPIC, "RPL_NOTOPIC", "%s :No topic is set"},
    {ERR_CHANOPRIVSNEEDED, "ERR_CHANOPRIVSNEEDED",
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
