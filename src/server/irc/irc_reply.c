#include <server/irc.h>

const t_irc_reply g_replys[] = {
    {RPL_WELCOME, TOSTR(RPL_WELCOME),
     ":Welcome to the Internet Relay Network %s!%s@%s"},
    {RPL_AWAY, TOSTR(RPL_AWAY), "%s :%s"},
    {RPL_UNAWAY, TOSTR(RPL_UNAWAY), ":You are no longer marked as being away"},
    {RPL_NOWAWAY, TOSTR(RPL_NOWAWAY), ":You have been marked as being away"},

    {RPL_NAMREPLY, TOSTR(RPL_NAMREPLY), "%s :%s"},
    {RPL_ENDOFNAMES, TOSTR(RPL_ENDOFNAMES), "%s :End of /NAMES list"},

    {ERR_NOSUCHNICK, TOSTR(ERR_NOSUCHNICK), "%s :No such nick/channel"},
    {ERR_NOSUCHSERVER, TOSTR(ERR_NOSUCHSERVER), "%s :No such server"},
    {ERR_NOSUCHCHANNEL, TOSTR(ERR_NOSUCHCHANNEL), "%s :No such channel"},
    {ERR_NICKNAMEINUSE, TOSTR(ERR_NICKNAMEINUSE),
     "%s :Nickname is already in use"},
    {ERR_ERRONEUSNICKNAME, TOSTR(ERR_ERRONEUSNICKNAME),
     "%s :Erroneus nickname"},
    {ERR_NONICKNAMEGIVEN, TOSTR(ERR_NONICKNAMEGIVEN), ":No nickname given"},
    {ERR_NEEDMOREPARAMS, TOSTR(ERR_NEEDMOREPARAMS),
     "%s :Not enough parameters"},
    {ERR_ALREADYREGISTRED, TOSTR(ERR_ALREADYREGISTRED),
     ":You may not reregister"},
    {ERR_NOTEXTTOSEND, TOSTR(ERR_NOTEXTTOSEND), ":No text to send"},
    {ERR_NOTONCHANNEL, TOSTR(ERR_NOTONCHANNEL),
     "%s :You're not on that channel"},
    {RPL_WHOISUSER, TOSTR(RPL_WHOISUSER), "%s %s %s * :%s"},
    {RPL_ENDOFWHOIS, TOSTR(RPL_ENDOFWHOIS), "%s :End of /WHOIS list"},
    {RPL_WHOISCHANNELS, TOSTR(RPL_WHOISCHANNELS), "%s :%s"},

    {RPL_WHOREPLY, TOSTR(RPL_WHOREPLY),
     "%s %s %s %s %s %s" /*"%s %s %s %s %s :%d %s"*/},
    {RPL_ENDOFWHO, TOSTR(RPL_ENDOFWHO), "%s :End of /WHO list"},
    {RPL_TOPIC, TOSTR(RPL_TOPIC), "%s :%s"},
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
            sprintf(msg, "%s %s \x0D\x0A", g_replys[i].name, g_replys[i].fmt);
            va_start(ap, code);
            cbuffer_putvcmd(&e->fds[cs].buf_write, msg, ap);
            va_end(ap);
            return (0);
        }
        i++;
    }

    return (-1);
}
