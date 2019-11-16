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
    [IRC_WHOIS] = {"WHOIS", &irc_whois},
};

int irc_command(t_env *e, int cs, size_t end_command_index)
{
    size_t  i;
    size_t  offset;
    t_token tokens[30];
    char    command[CBUFFSIZE + 1];

    memset(command, 0, CBUFFSIZE);

    // Copy circular buffer command into local buffer
    if (e->fds[cs].buf_read.tail < end_command_index)
        memcpy(command, e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
               end_command_index - e->fds[cs].buf_read.tail);
    else
    {
        memcpy(command, e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
               CBUFFSIZE - e->fds[cs].buf_read.tail);
        memcpy(command + CBUFFSIZE - e->fds[cs].buf_read.tail,
               e->fds[cs].buf_read.buffer, end_command_index);
    }

    // Skip withespaces
    offset = 0;
    while (command[offset] == 0x20)
        offset++;

    i = 0;
    while (i < IRC_COMMANDS_NUMBER)
    {
        if (strncmp(command + offset, g_irc_commands[i].command,
                    strlen(g_irc_commands[i].command)) == 0)
        {
            memset(tokens, 0, sizeof(t_token) * 30);

            // printf("ret:%ld\n", tokenize(command + offset, tokens, 30));
            // j = 0;
            // while (j < 30 && tokens[j].addr)
            //     printf("token:%s\n", tokens[j++].addr);
            tokenize(command + offset, tokens, 30);

            return g_irc_commands[i].f(e, cs, tokens);
        }
        i++;
    }

    logerror("Unknow command\n");

    return (-1);
}