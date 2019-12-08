#include <client/irc.h>
#include <client/ui/panel.h>

static const t_irc_cmd g_irc_commands[IRC_C2S_COMMANDS_NUMBER] = {
        [IRC_C2S_JOIN] = {"/join", &c2s_join},
        [IRC_C2S_NICK] = {"/nick", &c2s_nick},
        [IRC_C2S_MSG] = {"/msg", &c2s_msg},
        [IRC_C2S_CONNECT] = {"/connect", &c2s_connect},
        [IRC_C2S_WHO] = {"/who", &c2s_who},
        [IRC_C2S_PASS] = {"/pass", &c2s_pass},
        [IRC_C2S_AWAY] = {"/away", &c2s_away},
        [IRC_C2S_UNAWAY] = {"/unaway", &c2s_unaway},
        [IRC_C2S_LEAVE] = {"/leave", &c2s_leave},
        [IRC_C2S_WHOIS] = {"/whois", &c2s_whois},
        [IRC_C2S_LIST] = {"/list", &c2s_list},
        [IRC_C2S_TIME] = {"/time", &c2s_time},
        [IRC_C2S_VERSION] = {"/version", &c2s_version},
        [IRC_C2S_INVITE] = {"/invite", &c2s_invite},
        [IRC_C2S_TOPIC] = {"/topic", &c2s_topic},
};

/*
** eow stands for end of word
*/

static int is_eow(const char *buffer, size_t len)
{
    return (buffer[len] == ' ' || buffer[len] == '\n' || buffer[len] == 0);
}

int c2s(t_env *e, const char *buffer)
{
    size_t  i;
    t_token tokens[30];

    // Skip withespaces
    while (*buffer == 0x20 || *buffer == 0x0A)
        buffer++;

    if (*buffer == 0)
        return (0);

    i = 0;
    while (i < IRC_C2S_COMMANDS_NUMBER)
    {
        if (strncmp(buffer, g_irc_commands[i].command,
                    strlen(g_irc_commands[i].command)) == 0 &&
            is_eow(buffer, strlen(g_irc_commands[i].command)))
        {
            tokenize(buffer, tokens, 30);
            return (g_irc_commands[i].f(e, tokens));
        }
        i++;
    }

    if (e->sock == -1)
        return (irc_error(e, ERR_NOT_CONNECTED));

    if (buffer[0] == '/')
        return (irc_error(e, ERR_UNRECOGNIZED_COMMAND, buffer));

    _c2s_msg(e->self, e->self->channelname, strlen(e->self->channelname),
             buffer);

    return (0);
}
