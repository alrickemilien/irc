#include <ctype.h>
#include <server/irc.h>

static int irc_privmsg_check_command(t_env *e, int cs, const t_token *tokens)
{
    if (!tokens[1].addr || !tokens[1].len)
    {
        irc_reply(e, cs, ERR_NOSUCHNICK, NULL);
        return (-1);
    }

    if (!tokens[2].addr)
    {
        irc_reply(e, cs, ERR_NOTEXTTOSEND, NULL);
        return (-1);
    }

    if (tokens[2].addr[0] == ':' &&
        (tokens[2].addr[1] == 0 ||
         tokens[2].addr + 1 == strstr(tokens[2].addr, "\x0D\x0A")))
    {
        irc_reply(e, cs, ERR_NOTEXTTOSEND, NULL);
        return (-1);
    }

    return (0);
}

int irc_privmsg(t_env *e, int cs, t_token *tokens)
{
    size_t  i;
    size_t  j;
    t_token subtokens[30];

    if ((irc_privmsg_check_command(e, cs, tokens)) != 0)
        return (-1);

    memset(subtokens, 0, sizeof(t_token) * 30);

    tokenizechr(tokens[1].addr, subtokens, 30, ',');

    // printf("subtokens ret:%ld\n", tokenizechr(tokens[1].addr, subtokens, 30, ','));
    // j = 0;
    // while (j < 30 && subtokens[j].addr)
    //     printf("subtokens:%s\n", subtokens[j++].addr);

    // Find client to send private message
    i = 0;
    while (i <= e->max)
    {
        if (i != (size_t)cs && e->fds[i].type == FD_CLIENT &&
            e->fds[i].registered == 1)
        {
            j = 0;
            while (subtokens[j].addr)
            {
                if (strncmp(e->fds[i].nickname, subtokens[j].addr,
                            subtokens[j].len) == 0 ||
                    strncmp(e->channels[e->fds[i].channel].channel,
                            subtokens[j].addr, subtokens[j].len) == 0)
                {
                    if (e->fds[i].away)
                    {
                        irc_reply(e, cs, RPL_AWAY, e->fds[i].nickname,
                                  e->fds[i].awaymessage);
                    }
                    else
                    {
                        strcat(e->fds[i].buf_write, ":");
                        strcat(e->fds[i].buf_write, e->fds[cs].nickname);
                        strcat(e->fds[i].buf_write, " PRIVMSG ");
                        strcat(e->fds[i].buf_write, tokens[2].addr[0] == ':'
                                                        ? tokens[2].addr + 1
                                                        : tokens[2].addr);
                        break;
                    }
                }

                j++;
            }
        }
        i++;
    }

    // if (subtokens[j].addr == NULL)
    // {
    //     irc_reply(e, cs, ERR_NOSUCHNICK, e->fds[i].nickname);
    //     return (-1);
    // }

    return (IRC_PRIVMSG);
}
