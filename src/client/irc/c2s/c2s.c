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
        [IRC_LIST] = {"/list", &c2s_list},
        [IRC_TIME] = {"/time", &c2s_time},
        [IRC_VERSION] = {"/version", &c2s_version},
        [IRC_INVITE] = {"/invite", &c2s_invite},
        [IRC_TOPIC] = {"/topic", &c2s_topic},
};

/*
** eow stands for end of word
*/

static int is_eow(const char *buffer, size_t len)
{
    return (buffer[len] == ' ' || buffer[len] == '\n' || buffer[len] == 0);
}

int c2s(t_env *e, char *buffer)
{
    size_t  i;
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
            is_eow(buffer, strlen(g_irc_commands[i].command)))
        {
            tokenize(buffer, tokens, 30);
            return (g_irc_commands[i].f(e, tokens));
        }
        i++;
    }

    if (e->sock == -1 || buffer[0] == '/')
        return (irc_error(e, ERR_UNRECOGNIZED_COMMAND, buffer));

    _c2s_msg(e->self, e->self->channelname, strlen(e->self->channelname),
             buffer);

    return (0);
}
