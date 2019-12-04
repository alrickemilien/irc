#include <client/irc.h>
#include <client/ui/panel.h>

static int  s2c_rpl_who_state = 0;
static char s2c_rpl_who_buffer[512];

int s2c_rpl_whoreply(t_env *e, int cs, t_token *tokens)
{
    (void)cs;

    logdebug("s2c_rpl_whoreply:: %s", tokens[0].addr);

    if (s2c_rpl_who_state == 0)
    {
        memset(s2c_rpl_who_buffer, 0, sizeof(s2c_rpl_who_buffer));
        s2c_rpl_who_state = 1;
    }

    if (!tokens[1].addr)
        return (-1);

    // When buffer is full, flush buffer
    if (strlen(tokens[1].addr) >
        (sizeof(s2c_rpl_who_buffer) - strlen(s2c_rpl_who_buffer)))
    {
        loginfo(s2c_rpl_who_buffer);
        if (e->options.gui)
            ui_new_message(e->ui, s2c_rpl_who_buffer, UI_INFO_MSG);
        memset(s2c_rpl_who_buffer, 0, sizeof(s2c_rpl_who_buffer));
    }

    strcat(s2c_rpl_who_buffer, tokens[1].addr);
    strcat(s2c_rpl_who_buffer, " ");

    return (IRC_S2C_RPL_WHOREPLY);
}

int s2c_rpl_endofwho(t_env *e, int cs, t_token *tokens)
{
    (void)cs;
    (void)tokens;

    if (s2c_rpl_who_state == 0)
        return (-1);

    loginfo(s2c_rpl_who_buffer);

    s2c_rpl_who_state = 0;

    if (e->options.gui)
        ui_names(e->ui, s2c_rpl_who_buffer);

    return (IRC_S2C_RPL_ENDOFWHO);
}
