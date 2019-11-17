#include <client/irc.h>
#include <ctype.h>

/*
** the only restriction on a
** channel name is that it may not contain any spaces (' '), a control G
** (^G or ASCII 7), or a comma (',' which is used as a list item
** separator by the protocol)
*/

// static bool is_valid_chan(const char *channel)
// {
//     size_t i;

//     i = 1;
//     while (channel[i] && channel[i] != '\x0D')
//     {
//         if (channel[i] == '\x07' || channel[i] == '\x20' ||
//             channel[i] == '\x2C')
//             return (false);
//         i++;
//     }
//     return (true);
// }

static int irc_msg_check_command(t_env *e, int cs, const t_token *tokens)
{
    (void)cs;
    (void)e;

    if (!tokens[1].addr || !tokens[2].addr)
        return logerror("ERR_NEEDMOREPARAMS");
    return (0);
}

int irc_msg(t_env *e, int cs, t_token *tokens)
{
    if ((irc_msg_check_command(e, cs, tokens)) != 0)
        return (-1);

    cbuffer_putstr(&e->fds[cs].buf_write, "PRIVMSG ");
    cbuffer_put(&e->fds[cs].buf_write, (uint8_t*)tokens[1].addr, tokens[1].len);
    cbuffer_putstr(&e->fds[cs].buf_write, " :");
    cbuffer_put(&e->fds[cs].buf_write, (uint8_t*)tokens[2].addr, tokens[2].len);
    cbuffer_putstr(&e->fds[cs].buf_write, "\x0D\x0A");

    // loginfo("%s: %s", e->fds[cs].nickname, tokens[2].addr);

    return (IRC_JOIN);
}
