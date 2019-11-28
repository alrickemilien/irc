#include <client/irc.h>
#include <client/ui/panel.h>
#include <client/ui/login.h>
#include <ctype.h>

int s2c_rpl_welcome(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    void *tmp;

    if (!tokens[1].addr)
        return (-1);

    loginfo("%s\n",
            tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr);

    if (e->options.gui)
    {
        tmp = e->ui;

        e->ui = (t_ui_panel *)malloc(sizeof(t_ui_panel));

        if (e->ui == NULL || ui_init_panel_window(e, e->ui) < 0)
            return (-1);

        ui_set_nick(e->ui, e->fds[cs].nickname);
        set_user_name(e->ui, e->fds[cs].username);
        ui_set_status(e->ui, 0);

        gtk_widget_hide(((t_ui_login *)tmp)->window);
    }

    return (IRC_S2C_RPL_WELCOME);
}
