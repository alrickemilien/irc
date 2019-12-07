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
    {ERR_NICK_BAD_FORMAT, "/nick command bad formatted", NULL},
    {ERR_UNRECOGNIZED_COMMAND, "Unrecognized command '%s'", NULL},
    {0, NULL, NULL},
};

static int prefix_error(char *error_msg)
{
    char       t[ISOTIMESTRSIZE];
    static int is_tty = -1;

    if (is_tty == -1)
        is_tty = isatty(1);

    time2iso(t);
    return (sprintf(error_msg, is_tty ? "[%s] "
                                        "\x1b[31m"
                                        "ERROR: "
                                        "\x1b[0m"
                                      : "[%s] ERROR: ",
                    t));
}

int irc_error(t_env *e, int err_code, ...)
{
    size_t  i;
    char    error_msg[LOGSIZE + 1];
    va_list ap;

    memset(error_msg, 0, sizeof(error_msg));

    prefix_error(error_msg);

    i = 0;
    while (g_s2c_error[i].id)
    {
        if (g_s2c_error[i].id == err_code)
        {
            va_start(ap, err_code);

            vsprintf(error_msg + strlen(error_msg), g_s2c_error[i].fmt, ap);

            if (e->options.gui)
                ui_new_message(e->ui, error_msg, UI_ERROR_MSG);

            printf("%s\n", error_msg);

            va_end(ap);

            return (-1);
        }
        i++;
    }

    logerror("Unhandled error occured");
    if (e->options.gui)
        ui_new_message(e->ui, "Unhandled error occured", UI_ERROR_MSG);

    return (-1);
}
