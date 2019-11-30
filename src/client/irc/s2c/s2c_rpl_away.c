#include <client/irc.h>
#include <client/ui/panel.h>
#include <ctype.h>

int s2c_rpl_nowaway(t_env *e, int cs, t_token *tokens)
{
    logdebug("s2c_nowaway:: %s", tokens[0].addr);

    e->fds[cs].away = 1;

    if (e->options.gui)
        ui_away(e->ui, tokens[0].addr);

    return (IRC_S2C_RPL_NOWAWAY);
}

int s2c_rpl_unaway(t_env *e, int cs, t_token *tokens)
{
    logdebug("s2c_unaway:: %s", tokens[0].addr);

    e->fds[cs].away = 0;

    if (e->options.gui)
        ui_unaway(e->ui);

    return (IRC_S2C_RPL_UNAWAY);
}
