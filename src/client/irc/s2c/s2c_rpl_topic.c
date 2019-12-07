#include <client/irc.h>
#include <client/ui/panel.h>

int s2c_rpl_topic(t_env *e, t_token *tokens)
{
    (void)e;

    logdebug("s2c_rpl_topic:: %s", tokens[0].addr);

    if (!tokens[1].addr || !tokens[2].addr)
        return (-1);

    loginfo("TOPIC: %s",
            tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr);

    if (e->options.gui)
        ui_new_message(
            e->ui,
            tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr,
            UI_TOPIC_MSG);

    return (IRC_S2C_RPL_TOPIC);
}

int s2c_rpl_notopic(t_env *e, t_token *tokens)
{
    (void)e;


    if (!tokens[1].addr)
        return (-1);

    logdebug("s2c_rpl_notopic::");

    loginfo(tokens[1].addr);
    if (e->options.gui)
        ui_new_message(
            e->ui,
            tokens[1].addr,
            UI_TOPIC_MSG);

    return (IRC_S2C_RPL_NOTOPIC);
}
