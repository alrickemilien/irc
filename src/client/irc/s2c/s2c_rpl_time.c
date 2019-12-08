#include <client/irc.h>
#include <client/ui/panel.h>

static int s2c_rpl_time_check_command(t_env *e, const t_token *tokens)
{
    if (!tokens[1].addr || !tokens[2].addr)
        return (irc_error(e, ERR_NEEDMOREPARAMS));
    return (0);
}

int s2c_rpl_time(t_env *e, t_token *tokens)
{
    if (s2c_rpl_time_check_command(e, tokens) < 0)
        return (-1);

    loginfo("%s", tokens[1].addr);

    if (e->options.gui)
        ui_new_message(e->ui, tokens[1].addr, UI_INFO_MSG);

    return (IRC_S2C_RPL_TIME);
}
