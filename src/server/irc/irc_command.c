#include "server/irc.h"

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
    [IRC_JOIN] { "join", &irc_join },
};

void irc_command(t_env *e, int cs, const char *buffer)
{
    size_t i;

    if (buffer[0] != '/')
        return;

    i = 0;
    while (i < IRC_COMMANDS_NUMBER)
    {
        if (strncmp(buffer + 1, g_irc_commands[i].command,
                    strlen(buffer + 1)))
        {
            g_irc_commands[i].f(e, cs, &buffer);
            return;
        }
        i++;
    }

    strcpy(e->fds[cs].buf_write,
           "\x1b[31m"
           "ERROR"
           "\x1b[0m"
           " Unknow command\n");
}