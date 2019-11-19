#include <client/irc.h>

static const t_irc_cmd g_s2c_replys[IRC_S2C_COMMANDS_NUMBER] = {
        [IRC_S2C_RPL_WELCOME] = {"RPL_WELCOME", &s2c_rpl_welcome},
        [IRC_S2C_RPL_NAMREPLY] = {"RPL_NAMREPLY", &s2c_rpl_namreply},
        [IRC_S2C_RPL_ENDOFNAMES] = {"RPL_ENDOFNAMES", &s2c_rpl_endofnames},
        [IRC_S2C_PRIVMSG] = {"PRIVMSG", &s2c_privmsg},
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
        if (tokens[0].addr[0] == ':' && tokens[1].addr &&
            strncmp(tokens[1].addr, g_s2c_replys[i].command,
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

    return (-1);
}
