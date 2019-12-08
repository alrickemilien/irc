#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/panel.h>

int s2c_rpl_welcome(t_env *e, t_token *tokens)
{
    void *tmp;

    (void)tmp;

    if (!tokens[1].addr)
        return (-1);

    e->self->registered = 1;

    loginfo("%s",
            tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr);

    if (e->options.gui)
    {
        tmp = e->ui;

        e->ui = (t_ui_panel *)malloc(sizeof(t_ui_panel));

        if (e->ui == NULL || ui_init_panel(e, e->ui) < 0)
            return (-1);

        ui_set_nick(e->ui, e->self->nickname);
        ui_set_username(e->ui, e->self->username);
        ui_set_status(e->ui, 0);

        ui_clear_login_window(tmp);
    }

    return (IRC_S2C_RPL_WELCOME);
}
