#include <client/irc.h>
#include <client/ui/panel.h>

static int s2c_privmsg_check_command(t_env *e, int cs, const t_token *tokens)
{
    (void)cs;
    (void)e;

    if (!tokens[1].addr || !tokens[2].addr)
        return (irc_error(e, ERR_NEEDMOREPARAMS));
    return (0);
}

int s2c_privmsg(t_env *e, int cs, t_token *tokens)
{
    char msg[512];

    if (s2c_privmsg_check_command(e, cs, tokens) < 0)
        return (-1);

    memset(msg, 0, sizeof(msg));

    strncat(msg, tokens[0].addr[0] == ':' ? tokens[0].addr + 1 : tokens[0].addr,
            tokens[0].addr[0] == ':' ? tokens[0].len - 1 : tokens[0].len);
    strcat(msg, ": ");
    strcat(msg, tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr);

    loginfo("s2c_privmsg::msg:: %s", msg);

    if (e->options.gui)
        ui_new_message(e->ui, msg, UI_CHAT_MSG);

    return (IRC_S2C_RPL_WELCOME);
}
