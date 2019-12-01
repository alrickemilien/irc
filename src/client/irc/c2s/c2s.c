#include <client/irc.h>
#include <client/ui/panel.h>

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
    [IRC_JOIN] = {"/join", &c2s_join},
    [IRC_NICK] = {"/nick", &c2s_nick},
    [IRC_MSG] = {"/msg", &c2s_msg},
    [IRC_CONNECT] = {"/connect", &c2s_connect},
    [IRC_WHO] = {"/who", &c2s_who},
    [IRC_PASS] = {"/pass", &c2s_pass},
    [IRC_AWAY] = {"/away", &c2s_away},
    [IRC_UNAWAY] = {"/unaway", &c2s_unaway},
    [IRC_LEAVE] = {"/leave", &c2s_leave},
    [IRC_WHOIS] = {"/whois", &c2s_whois},
};

static int is_valid_termination(const char *buffer, size_t len)
{
    return (buffer[len] == ' ' || buffer[len] == '\n' || buffer[len] == 0);
}

int c2s(t_env *e, int cs, char *buffer)
{
    size_t i;
    // size_t  j;
    t_token tokens[30];

    // Skip withespaces
    while (*buffer == 0x20 || *buffer == 0x0A)
        buffer++;

    if (*buffer == 0)
        return (0);

    i = 0;
    while (i < IRC_COMMANDS_NUMBER)
    {
        if (strncmp(buffer, g_irc_commands[i].command,
                    strlen(g_irc_commands[i].command)) == 0 &&
            is_valid_termination(buffer, strlen(g_irc_commands[i].command)))
        {
            // printf("buffer: %s\n", buffer);

            memset(tokens, 0, sizeof(t_token) * 30);

            tokenize(buffer, tokens, 30);

            // logdebug("ret:%ld\n", tokenize(buffer, tokens, 30));
            // j = 0;
            // while (j < 30 && tokens[j].addr)
            // {
            //     logdebug("token:%s - token_len:%ld\n",
            //     tokens[j].addr,tokens[j].len); j++;
            // }

            return (g_irc_commands[i].f(e, cs, tokens));
        }
        i++;
    }

    if (e->sock == -1 || buffer[0] == '/')
        return (0);

    // logdebug("Unknow command '%s', treat as a msg to current channel", buffer);

    _c2s_msg(&e->fds[e->sock], e->fds[e->sock].channelname,
             strlen(e->fds[e->sock].channelname), buffer);

    return (0);
}
