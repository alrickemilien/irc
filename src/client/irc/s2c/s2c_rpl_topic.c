#include <client/irc.h>
#include <ctype.h>

int s2c_rpl_topic(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    if (!tokens[1].addr)
        return (-1);

    loginfo("TOPIC: %s\n",
            tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr);

    ui_topic(e->ui, tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr);

    return (IRC_S2C_RPL_TOPIC);
}
