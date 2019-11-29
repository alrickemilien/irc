#include <client/irc.h>
#include <client/ui/panel.h>
#include <ctype.h>

int s2c_rpl_topic(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    if (!tokens[1].addr)
        return (-1);

    loginfo("TOPIC: %s\n",
            tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr);

    if (e->options.gui)
        ui_new_message(
            e->ui,
            tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr,
            UI_TOPIC_MSG);

    return (IRC_S2C_RPL_TOPIC);
}
