#include <client/irc.h>

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
    [IRC_JOIN] = {"/join", &irc_join},
};

int irc_command(t_env *e, int cs, char *buffer)
{
    size_t  i;
    t_token tokens[30];

    // Skip withespaces
    while (*buffer == 0x20)
        buffer++;

    i = 0;
    while (i < IRC_COMMANDS_NUMBER)
    {
        if (strncmp(buffer, g_irc_commands[i].command,
                    strlen(g_irc_commands[i].command)) == 0)
        {
            memset(tokens, 0, sizeof(t_token) * 30);

            tokenize(buffer, tokens, 30);

            return g_irc_commands[i].f(e, cs, tokens);
        }
        i++;
    }

    printf(e->is_tty ? "\x1b[31m"
                       "ERROR"
                       "\x1b[0m"
                       " Unknow command\n"
                     : "ERROR Unknow command\n");
    return (-1);
}