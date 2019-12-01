#include <ctype.h>
#include <server/irc.h>

static int irc_whois_check_command(t_env *e, int cs, const t_token *tokens)
{
    if (!tokens[1].addr)
        return (irc_err(e, cs, ERR_NONICKNAMEGIVEN, NULL));
    return (0);
}

int irc_whois(t_env *e, int cs, t_token *tokens)
{
    size_t  i;
    size_t  j;
    size_t  subtoken_count;
    t_token subtokens[30];

    loginfo("irc_whois:: %s\n", tokens[0].addr);

    if (irc_whois_check_command(e, cs, tokens) < 0)
        return (-1);

    memset(subtokens, 0, sizeof(t_token) * 30);

    subtoken_count = tokenizechr(tokens[1].addr, subtokens, 30, ',');

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1)
        {
            j = 0;
            while (subtokens[j].addr)
            {
                if (strncmp(e->fds[i].nickname, subtokens[j].addr,
                            subtokens[j].len) == 0)
                {
                    irc_reply(e, cs, RPL_WHOISUSER, e->fds[i].nickname,
                              e->fds[i].username, e->fds[i].host,
                              e->fds[i].realname);
                    // irc_reply(e, cs, RPL_WHOISOPERATOR,
                    // e->channels[i].channel);
                    // irc_reply(e, cs, RPL_WHOISIDLE, e->channels[i].channel);
                    irc_reply(e, cs, RPL_WHOISCHANNELS, e->fds[i].nickname,
                              e->channels[e->fds[i].channel].channel);
                    irc_reply(e, cs, RPL_ENDOFWHOIS, e->fds[i].nickname);

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
        if (subtokens[j].addr != NULL)
            irc_err(e, cs, ERR_NOSUCHNICK, subtokens[j].addr);
        j++;
    }

    return (IRC_WHOIS);
}
