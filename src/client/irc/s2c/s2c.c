#include <client/irc.h>
#include <client/ui/panel.h>

static const t_irc_cmd g_s2c_replys[IRC_S2C_COMMANDS_NUMBER] = {
    [IRC_S2C_RPL_WELCOME] = {"RPL_WELCOME", &s2c_rpl_welcome},
    [IRC_S2C_RPL_NAMREPLY] = {"RPL_NAMREPLY", &s2c_rpl_namreply},
    [IRC_S2C_RPL_ENDOFNAMES] = {"RPL_ENDOFNAMES", &s2c_rpl_endofnames},
    [IRC_S2C_PRIVMSG] = {"PRIVMSG", &s2c_privmsg},
    [IRC_S2C_JOIN] = {"JOIN", &s2c_join},
    [IRC_S2C_NICK] = {"NICK", &s2c_nick},
    [IRC_S2C_RPL_NOWAWAY] = {"RPL_NOWAWAY", &s2c_rpl_nowaway},
    [IRC_S2C_RPL_UNAWAY] = {"RPL_UNAWAY", &s2c_rpl_unaway},
    [IRC_S2C_RPL_TOPIC] = {"RPL_TOPIC", &s2c_rpl_topic},
    [IRC_S2C_RPL_WHOISUSER] = {"RPL_WHOISUSER", &s2c_rpl_whoisuser},
    [IRC_S2C_RPL_WHOISCHANNELS] = {"RPL_WHOISCHANNELS", &s2c_rpl_whoischannels},
    [IRC_S2C_RPL_ENDOFWHOIS] = {"RPL_ENDOFWHOIS", &s2c_rpl_endofwhois},
};

int s2c(t_env *e, int cs, char *buffer)
{
    size_t i;
    // size_t  j;
    t_token tokens[30];

    // Skip withespaces
    while (*buffer == 0x20)
        buffer++;

    memset(tokens, 0, sizeof(t_token) * 30);

    tokenize(buffer, tokens, 30);

    // logdebug("ret:%ld\n", tokenize(buffer, tokens, 30));
    // j = 0;
    // while (j < 30 && tokens[j].addr)
    // {
    //     logdebug("token:%s - token_len:%ld\n",
    //     tokens[j].addr,tokens[j].len);
    //     j++;
    // }

    i = 0;
    while (tokens[0].addr && i < IRC_S2C_COMMANDS_NUMBER)
    {
        if (tokens[1].addr && strncmp(tokens[1].addr, g_s2c_replys[i].command,
                                      strlen(g_s2c_replys[i].command)) == 0)
        {
            return g_s2c_replys[i].f(e, cs, tokens);
        }

        if (strncmp(tokens[0].addr, g_s2c_replys[i].command,
                    strlen(g_s2c_replys[i].command)) == 0)
        {
            return g_s2c_replys[i].f(e, cs, tokens);
        }
        i++;
    }

    logerror("Unknow command %s\n", buffer);

    if (e->options.gui)
        ui_new_message(
            e->ui,
            tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr,
            UI_ERROR_MSG);

    return (-1);
}
