#include "server/irc.h"

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
    [IRC_JOIN] { "join", &irc_join },
};

void irc_command(t_env *e, int cs, char *buffer)
{
    size_t i;
    size_t j;
    char * tokens[30];

    if (buffer[0] != '/')
        return;

    i = 0;
    while (i < IRC_COMMANDS_NUMBER)
    {
        if (strncmp(buffer + 1, g_irc_commands[i].command, strlen(buffer + 1)))
        {
            memset(tokens, 0, sizeof(char *) * 30);

            printf("ret:%ld\n", tokenize(buffer, tokens, 30));
            j = 0;
            while (j < 30 && tokens[j])
                printf("token:%s\n", tokens[j++]);

            printf("ici\n");

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