#include <client/irc.h>
#include <client/ui/panel.h>

int s2c_rpl_inviting(t_env *e, t_token *tokens)
{
    char msg[512];

    memset(msg, 0, sizeof(msg));

    sprintf(msg, "You invited %.*s to channel %.*s", (int)tokens[1].len, tokens[1].addr,
            (int)tokens[2].len, tokens[2].addr);

    loginfo(msg);

    if (e->options.gui)
        ui_new_message(e->ui, msg, UI_INFO_MSG);

    return (IRC_S2C_RPL_INVITING);
}
