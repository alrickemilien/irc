#include <client/irc.h>
#include <client/ui/panel.h>

static const t_irc_cmd g_s2c_replys[IRC_S2C_COMMANDS_NUMBER] = {
    [IRC_S2C_RPL_WELCOME] = {"001", &s2c_rpl_welcome},
    [IRC_S2C_RPL_NAMREPLY] = {"353", &s2c_rpl_namreply},
    [IRC_S2C_RPL_ENDOFNAMES] = {"366", &s2c_rpl_endofnames},
    [IRC_S2C_PRIVMSG] = {"PRIVMSG", &s2c_privmsg},
    [IRC_S2C_JOIN] = {"JOIN", &s2c_join},
    [IRC_S2C_NICK] = {"NICK", &s2c_nick},
    [IRC_S2C_RPL_NOWAWAY] = {"306", &s2c_rpl_nowaway},
    [IRC_S2C_RPL_UNAWAY] = {"305", &s2c_rpl_unaway},
    [IRC_S2C_RPL_TOPIC] = {"332", &s2c_rpl_topic},
    [IRC_S2C_RPL_WHOISUSER] = {"311", &s2c_rpl_whoisuser},
    [IRC_S2C_RPL_WHOISCHANNELS] = {"319", &s2c_rpl_whoischannels},
    [IRC_S2C_RPL_ENDOFWHOIS] = {"318", &s2c_rpl_endofwhois},
    [IRC_S2C_RPL_WHOREPLY] = {"352", &s2c_rpl_whoreply},
    [IRC_S2C_RPL_ENDOFWHO] = {"315", &s2c_rpl_endofwho},
    [IRC_S2C_RPL_AWAY] = {"301", &s2c_rpl_away},
    [IRC_S2C_RPL_LISTSTART] = {"321", &s2c_rpl_liststart},
    [IRC_S2C_RPL_LISTEND] = {"323", &s2c_rpl_listend},
    [IRC_S2C_RPL_LIST] = {"322", &s2c_rpl_list},
    [IRC_S2C_NOTICE] = {"NOTICE", &s2c_notice},
    [IRC_S2C_RPL_TIME] = {"391", &s2c_rpl_time},
    [IRC_S2C_PING] = {"PING", &s2c_ping},
    [IRC_S2C_RPL_VERSION] = {"351", &s2c_rpl_version},
    [IRC_S2C_RPL_INVITING] = {"341", &s2c_rpl_inviting},
    [IRC_S2C_INVITE] = {"INVITE", &s2c_invite},
    [IRC_S2C_RPL_NOTOPIC] = {"331", &s2c_rpl_notopic},
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

    logdebug("s2c :: buffer :: %s", buffer);


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

    if (e->options.gui && e->self->registered)
        ui_new_message(e->ui, buffer, UI_ERROR_MSG);

    return (-1);
}
