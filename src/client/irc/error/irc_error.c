#include <client/irc.h>
#include <client/ui/panel.h>

static const t_s2c_error g_s2c_error[] = {
    {ERR_NOSUCHCHANNEL, "Invalid channel '%s'", NULL},
    {ERR_NEEDMOREPARAMS, "Not enough parameters for command '%s'", NULL},
    {ERR_NICKNAMEINUSE, "Nickname '%s' already in use", NULL},
    {ERR_NONICKNAMEGIVEN, "No nickname provided command", NULL},
    {ERR_ERRONEUSNICKNAME, "Invalid character in nickname '%s'", NULL},
    {ERR_NOT_CONNECTED,
     "You need to be logged in before any command. Use "
     "/connect [server] ?[port]",
     NULL},
     {ERR_NOORIGIN, ":No origin specified", NULL},
    {ERR_NICK_BAD_FORMAT, "/nick command bad formatted" , NULL},
    {ERR_UNRECOGNIZED_COMMAND, "Unrecognized command '%s'", NULL},
    {0, NULL, NULL},
};

int irc_error(t_env *e, int err_code, ...)
{
    size_t  i;
    char    error_msg[512];
    va_list ap;

    i = 0;
    while (g_s2c_error[i].id)
    {
        if (g_s2c_error[i].id == err_code)
        {
            va_start(ap, err_code);

            printf("a\n");

            vsprintf(error_msg, g_s2c_error[i].fmt, ap);
            printf("b\n");

            if (e->options.gui)
                ui_new_message(e->ui, error_msg, UI_ERROR_MSG);
            printf("c\n");

            printf("error_msg: %s\n", error_msg);
            
            logerror(error_msg);
            printf("d\n");

            va_end(ap);
            printf("e\n");

            return (-1);
        }
        i++;
    }

    logerror("Unhandled error occured");
    if (e->options.gui)
        ui_new_message(e->ui, "Unhandled error occured", UI_ERROR_MSG);

    return (-1);
}
