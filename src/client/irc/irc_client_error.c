#include <client/irc.h>
#include <client/ui/ui.h>

int irc_client_error(t_env *e, const char *fmt, ...)
{
    if (e->options.gui)
        ui_new_message(e->ui, msg, UI_ERROR_MSG);

    return (vlogerror(fmt, buffer));
}