#include "server/irc.h"

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
    [IRC_JOIN]= { "JOIN", &irc_join }, [IRC_MSG]= { "MSG", &irc_msg },
};

void irc_command(t_env *e, int cs, char *buffer)
{
    size_t i;
    // size_t  j;
    t_token tokens[30];

    i = 0;
    while (i < IRC_COMMANDS_NUMBER)
    {
        if (strncmp(buffer, g_irc_commands[i].command,
                    strlen(g_irc_commands[i].command)) == 0)
        {
            memset(tokens, 0, sizeof(t_token) * 30);

            // printf("ret:%ld\n", tokenize(buffer + 1, tokens, 30));
            tokenize(buffer, tokens, 30);

            // j = 0;
            // while (j < 30 && tokens[j].addr)
            //     printf("token:%s\n", tokens[j++].addr);

            g_irc_commands[i].f(e, cs, tokens);
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