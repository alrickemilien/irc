#include <client/irc.h>
#include <ctype.h>

static int  s2c_rpl_namreply_state = 0;
static char s2c_rpl_namreply_buffer[512];

int s2c_rpl_namreply(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    if (s2c_rpl_namreply_state == 0)
    {
        memset(s2c_rpl_namreply_buffer, 0, sizeof(s2c_rpl_namreply_buffer));
        s2c_rpl_namreply_state = 1;
    }

    if (!tokens[1].addr)
        return (-1);

    strcat(s2c_rpl_namreply_buffer, tokens[1].addr);

    return (IRC_S2C_RPL_NAMREPLY);
}

int s2c_rpl_endofnames(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;
    (void)tokens;

    if (s2c_rpl_namreply_state == 0)
        return (-1);

    s2c_rpl_namreply_state = 0;

    loginfo(s2c_rpl_namreply_buffer);

    return (IRC_S2C_RPL_ENDOFNAMES);
}