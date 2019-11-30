#include <client/irc.h>
#include <client/ui/panel.h>
#include <ctype.h>

static int  s2c_rpl_whois_state = 0;
static char s2c_rpl_whois_buffer[512];

int s2c_rpl_whoisuser(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    logdebug("s2c_rpl_whoisuser:: %s", tokens[0].addr);

    if (s2c_rpl_whois_state == 0)
    {
        memset(s2c_rpl_whois_buffer, 0, sizeof(s2c_rpl_whois_buffer));
        s2c_rpl_whois_state = 1;
    }

    if (!tokens[1].addr)
        return (-1);

    strcat(s2c_rpl_whois_buffer, tokens[1].addr);
    strcat(s2c_rpl_whois_buffer, " ");

    return (IRC_S2C_RPL_WHOISUSER);
}

int s2c_rpl_whoischannels(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;

    logdebug("s2c_rpl_whoischannels:: %s", tokens[0].addr);

    if (s2c_rpl_whois_state == 0)
        return (-1);

    strcat(s2c_rpl_whois_buffer, tokens[1].addr);
    strcat(s2c_rpl_whois_buffer, " ");

    return (IRC_S2C_RPL_WHOISCHANNELS);
}

int s2c_rpl_endofwhois(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)cs;
    (void)tokens;

    logdebug("s2c_rpl_endofwhois:: %s", tokens[0].addr);

    if (s2c_rpl_whois_state == 0)
        return (-1);

    loginfo("%s\n", s2c_rpl_whois_buffer);

    s2c_rpl_whois_state = 0;

    if (e->options.gui)
        ui_whois(e->ui, s2c_rpl_whois_buffer);

    return (IRC_S2C_RPL_ENDOFWHOIS);
}
