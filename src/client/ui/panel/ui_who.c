#include <client/ui/panel.h>

int ui_whois(t_ui_panel *ui, const char *msg)
{
    ui_new_message(ui, msg, UI_INFO_MSG);
    return (0);
}
