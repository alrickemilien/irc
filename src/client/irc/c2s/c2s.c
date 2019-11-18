#include <client/irc.h>

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
    [IRC_JOIN] = {"/join", &c2s_join},
    [IRC_NICK] = {"/nick", &c2s_nick},
    [IRC_MSG] = {"/msg", &c2s_msg},
    [IRC_CONNECT] = {"/connect", &c2s_connect},
};

int c2s(t_env *e, int cs, char *buffer)
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

    logerror("Unknow command %s\n", buffer);

    return (-1);
}
