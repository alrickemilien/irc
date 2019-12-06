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
    [IRC_S2C_RPL_WHOREPLY] = {"RPL_WHOREPLY", &s2c_rpl_whoreply},
    [IRC_S2C_RPL_ENDOFWHO] = {"RPL_ENDOFWHO", &s2c_rpl_endofwho},
    [IRC_S2C_RPL_AWAY] = {"RPL_AWAY", &s2c_rpl_away},
    [IRC_S2C_RPL_LISTSTART] = {"RPL_LISTSTART", &s2c_rpl_liststart},
    [IRC_S2C_RPL_LISTEND] = {"RPL_LISTEND", &s2c_rpl_listend},
    [IRC_S2C_RPL_LIST] = {"RPL_LIST", &s2c_rpl_list},
    [IRC_S2C_NOTICE] = {"NOTICE", &s2c_notice},
    [IRC_S2C_RPL_TIME] = {"RPL_TIME", &s2c_rpl_time},
    [IRC_S2C_PING] = {"PING", &s2c_ping},
    [IRC_S2C_RPL_VERSION] = {"RPL_VERSION", &s2c_rpl_version},
    [IRC_S2C_RPL_INVITING] = {"RPL_INVITING", &s2c_rpl_inviting},
    [IRC_S2C_INVITE] = {"INVITE", &s2c_invite},
};

int s2c(t_env *e, char *buffer)
{
    size_t i;
    // size_t  j;
    t_token tokens[30];

    // Skip withespaces
    while (*buffer == 0x20)
        buffer++;

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
            return g_s2c_replys[i].f(e, tokens);
        }

        if (strncmp(tokens[0].addr, g_s2c_replys[i].command,
                    strlen(g_s2c_replys[i].command)) == 0)
        {
            return g_s2c_replys[i].f(e, tokens);
        }
        i++;
    }

    logerror("Unknow command %s", buffer);

    if (e->options.gui)
        ui_new_message(e->ui, buffer, UI_ERROR_MSG);

    return (-1);
}
