#include <client/irc.h>
#include <client/ui/panel.h>

int s2c_rpl_version(t_env *e, t_token *tokens)
{
    loginfo(tokens[1].addr);

    if (e->options.gui)
        ui_new_message(e->ui, tokens[1].addr, UI_INFO_MSG);

    return (IRC_S2C_RPL_VERSION);
}
