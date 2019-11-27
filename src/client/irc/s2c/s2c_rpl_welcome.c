#include <client/irc.h>
#include <client/ui/panel.h>
#include <ctype.h>

int s2c_rpl_welcome(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    if (!tokens[1].addr)
        return (-1);

    loginfo("%s\n",
            tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr);

    if (e->options.gui)
    {
        set_nick_name(e->ui, e->fds[cs].nickname);
        set_user_name(e->ui, e->fds[cs].username);
        set_status(e->ui, 0);
    }

    return (IRC_S2C_RPL_WELCOME);
}
