#include <server/irc.h>

static int irc_notice_check_command(t_env *e, int cs, const t_token *tokens)
{
    if (!tokens[1].addr || !tokens[1].len)
        return (irc_err(e, cs, ERR_NOSUCHNICK, NULL));
    if (!tokens[2].addr)
        return (irc_err(e, cs, ERR_NOTEXTTOSEND, NULL));
    if (tokens[2].addr[0] == ':' &&
        (tokens[2].addr[1] == 0 ||
         tokens[2].addr + 1 == strstr(tokens[2].addr, "\x0D\x0A")))
        return (irc_err(e, cs, ERR_NOTEXTTOSEND, NULL));
    return (0);
}

int irc_notice(t_env *e, int cs, t_token *tokens)
{
    size_t  i;
    size_t  j;
    size_t  subtoken_count;
    t_token subtokens[30];

    if ((irc_notice_check_command(e, cs, tokens)) < 0)
        return (-1);

    memset(subtokens, 0, sizeof(t_token) * 30);

    subtoken_count = tokenizechr(tokens[1].addr, subtokens, 30, ',');

    // Find client to send private message
    i = 0;
    while (i <= e->max)
    {
        if (i != (size_t)cs && e->fds[i].type == FD_CLIENT &&
            e->fds[i].registered == 1)
        {
            j = 0;
            while (j < subtoken_count)
            {
                if (subtokens[j].addr &&
                    (strncmp(e->fds[i].nickname, subtokens[j].addr,
                             subtokens[j].len) == 0 ||
                     strncmp(e->channels[e->fds[i].channel].channel,
                             subtokens[j].addr, subtokens[j].len) == 0))
                {
                    if (!e->fds[i].away)
                    {
                        cbuffer_putstr(&e->fds[i].buf_write, ":");
                        cbuffer_putstr(&e->fds[i].buf_write,
                                       e->fds[cs].nickname);
                        cbuffer_putstr(&e->fds[i].buf_write, " PRIVMSG :");
                        cbuffer_putstr(&e->fds[i].buf_write,
                                       tokens[2].addr[0] == ':'
                                           ? tokens[2].addr + 1
                                           : tokens[2].addr);
                        cbuffer_putstr(&e->fds[i].buf_write, "\x0D\x0A");
                    }

                    // Set to NULL only clients, to channels
                    if (subtokens[j].addr[0] != '&' &&
                        subtokens[j].addr[0] != '#')
                        subtokens[j].addr = (void *)0;

                    break;
                }
                j++;
            }
        }
        i++;
    }
    irc_privmsg_nomatch_nick(e, cs, subtokens, subtoken_count);
    return (IRC_PRIVMSG);
}
