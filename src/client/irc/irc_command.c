#include <client/irc.h>

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
    [IRC_JOIN] = {"/join", &irc_join},
    [IRC_NICK] = {"/nick", &irc_nick},
    [IRC_MSG] = {"/msg", &irc_msg},
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

            printf("ici: %s\n", g_irc_commands[i].command);
            memset(tokens, 0, sizeof(t_token) * 30);

            tokenize(buffer, tokens, 30);

            return g_irc_commands[i].f(e, cs, tokens);
        }
        i++;
    }

    logerror("Unknow command\n");

    return (-1);
}