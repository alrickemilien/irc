#include "server/irc.h"

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
        [IRC_JOIN] = {"JOIN", &irc_join},
        [IRC_NICK] = {"NICK", &irc_nick},
        [IRC_USER] = {"USER", &irc_user},
        [IRC_QUIT] = {"QUIT", &irc_quit},
        [IRC_NAMES] = {"NAMES", &irc_names},
        [IRC_PRIVMSG] = {"PRIVMSG", &irc_privmsg},
        [IRC_NOTICE] = {"NOTICE", &irc_privmsg},
        [IRC_PASS] = {"PASS", &irc_pass},
        [IRC_AWAY] = {"AWAY", &irc_away},
        [IRC_PART] = {"PART", &irc_part},
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

            // printf("ret:%ld\n", tokenize(buffer + 1, tokens, 30));
            tokenize(buffer, tokens, 30);

            return g_irc_commands[i].f(e, cs, tokens);
        }
        i++;
    }

    logerror("Unknow command\n");
    
    return (-1);
}