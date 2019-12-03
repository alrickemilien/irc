#include <client/irc.h>
#include <client/ui/panel.h>

static int  s2c_rpl_listrply_state = 0;
static char s2c_rpl_listrply_buffer[512];

int s2c_rpl_liststart(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;
    (void)tokens;

    if (s2c_rpl_listrply_state == 0)
    {
        memset(s2c_rpl_listrply_buffer, 0, sizeof(s2c_rpl_listrply_buffer));
        s2c_rpl_listrply_state = 1;
    }

    // if (!tokens[1].addr)
    // return (-1);

    // strcat(s2c_rpl_listrply_buffer, tokens[1].addr);

    return (IRC_S2C_RPL_LISTSTART);
}

int s2c_rpl_list(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    if (s2c_rpl_listrply_state == 0)
        return (-1);

    if (!tokens[1].addr)
        return (-1);

    if (s2c_rpl_listrply_buffer[0] != 0)
        strcat(s2c_rpl_listrply_buffer, "\n");

    strcat(s2c_rpl_listrply_buffer, tokens[1].addr);

    return (IRC_S2C_RPL_LIST);
}

int s2c_rpl_listend(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;
    (void)tokens;

    if (s2c_rpl_listrply_state == 0)
        return (-1);

    s2c_rpl_listrply_state = 0;

    loginfo(s2c_rpl_listrply_buffer);

    if (e->options.gui)
        ui_new_message(e->ui, s2c_rpl_listrply_buffer, UI_INFO_MSG);

    return (IRC_S2C_RPL_LISTEND);
}
