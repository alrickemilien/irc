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
    size_t  subtoken_count;
    t_token subtokens[30];

    if ((irc_privmsg_check_command(e, cs, tokens)) != 0)
        return (-1);

    memset(subtokens, 0, sizeof(t_token) * 30);

    logdebug("irc_privmsg:: %s\n", tokens[0].addr);

    subtoken_count = tokenizechr(tokens[1].addr, subtokens, 30, ',');

    // printf("subtokens ret:%ld\n", tokenizechr(tokens[1].addr, subtokens, 30,
    // ',')); j = 0; while (j < 30 && subtokens[j].addr)
    //     printf("subtokens:%s\n", subtokens[j++].addr);

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
                logdebug("irc_privmsg:: subtokens[j]: %s\n", subtokens[j].addr);
                if (subtokens[j].addr &&
                    (strncmp(e->fds[i].nickname, subtokens[j].addr,
                             subtokens[j].len) == 0 ||
                     strncmp(e->channels[e->fds[i].channel].channel,
                             subtokens[j].addr, subtokens[j].len) == 0))
                {
                    logdebug("irc_privmsg:: sending message to %ld\n", i);

                    if (e->fds[i].away)
                    {
                        irc_reply(e, cs, RPL_AWAY, e->fds[i].nickname,
                                  e->fds[i].awaymessage);
                    }
                    else
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

    // Error on nick that match nothing
    j = 0;
    while (j < subtoken_count)
    {
        if (subtokens[j].addr != NULL && subtokens[j].addr[0] != '&' &&
            subtokens[j].addr[0] != '#')
            irc_reply(e, cs, ERR_NOSUCHNICK, subtokens[j].addr);
        j++;
    }

    return (IRC_PRIVMSG);
}
