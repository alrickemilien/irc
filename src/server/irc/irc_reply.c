#include <server/irc.h>

const t_irc_reply replys[] = {
    {ERR_NOSUCHNICK, TOSTR(ERR_NOSUCHNICK), "%s :No such nick/channel"},
    {ERR_NOSUCHSERVER, TOSTR(ERR_NOSUCHSERVER), "%s :No such server"},
    {ERR_NOSUCHCHANNEL, TOSTR(ERR_NOSUCHCHANNEL), "%s :No such channel"},
    {ERR_NICKNAMEINUSE, TOSTR(ERR_NICKNAMEINUSE), "%s :Nickname is already in use"},
    {ERR_ERRONEUSNICKNAME, TOSTR(ERR_ERRONEUSNICKNAME), "%s :Erroneus nickname"},
    {ERR_NONICKNAMEGIVEN, TOSTR(ERR_NONICKNAMEGIVEN), ":No nickname given"},
    {ERR_NEEDMOREPARAMS, TOSTR(ERR_NEEDMOREPARAMS), "%s :Not enough parameters"},
    {ERR_ALREADYREGISTRED, TOSTR(ERR_ALREADYREGISTRED), ":You may not reregister"},
    {ERR_NOTEXTTOSEND, TOSTR(ERR_NOTEXTTOSEND), ":No text to send"},
};

int irc_reply(t_env *e, int cs, int code, const char *data)
{
    size_t i;
    char   msg[512];

    i = 0;
    while (i < sizeof(replys) / sizeof(t_irc_reply))
    {
        if (replys[i].code == code)
        {
            strcat(e->fds[cs].buf_write, replys[i].name);
            strcat(e->fds[cs].buf_write, " ");
            sprintf(msg, replys[i].fmt, data);
            strcat(e->fds[cs].buf_write, msg);

            return (0);
        }
        i++;
    }

    return (-1);
}