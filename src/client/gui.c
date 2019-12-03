#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/panel.h>

int gui(t_env *e, int argc, char **argv)
{
    gtk_init(&argc, &argv);

    e->ui = (t_ui_login *)malloc(sizeof(t_ui_login));

    if (ui_init_login_window(e, e->ui) < 0)
        return (-1);

    gtk_main();

    return (0);
}