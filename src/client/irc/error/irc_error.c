#include <client/irc.h>
#include <client/ui/panel.h>

static const t_s2c_error g_s2c_error[] = {
    {ERR_NOSUCHCHANNEL, "Invalid channel '%s'", NULL},
    {ERR_NEEDMOREPARAMS, "Not enough parameters for command '%s'", NULL},
    {ERR_NICKNAMEINUSE, "Nickname '%s' already in use", NULL},
    {ERR_NONICKNAMEGIVEN, "No nickname provided to /nick command", NULL},
    {ERR_ERRONEUSNICKNAME, "Invalid character in nickname '%s'", NULL},
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

            vsprintf(error_msg, g_s2c_error[i].fmt, ap);

            if (e->options.gui)
                ui_new_message(e->ui, error_msg, UI_ERROR_MSG);
            logerror(error_msg);

            va_end(ap);

            return (-1);
        }
        i++;
    }

    if (e->options.gui)
        ui_new_message(e->ui, "Unhandled error occured", UI_ERROR_MSG);
    logerror("Unhandled error occured");

    return (-1);
}