#include <ctype.h>
#include "server/irc.h"

static bool is_valid_chan(const char *channel)
{
    char *tmp;

    memcpy(&tmp, &channel, sizeof(char *));

    tmp++;
    while (*tmp && *tmp != '\n')
    {
        if (!isalnum(*tmp))
            return (false);
        tmp++;
    }
    return (true);
}

static int irc_join_check_command(t_env *e, int cs, const t_token *tokens)
{
    const char *channel;
    size_t      channel_len;

    if (!tokens[0].addr || !tokens[1].addr || tokens[2].addr)
    {
        strcpy(e->fds[cs].buf_write,
               "\x1b[31mERROR\x1b[0m"
               " Usage: JOIN <#channel>\n");
        return (-1);
    }

    channel = tokens[1].addr;
    channel_len = tokens[1].len;

    printf("channel : %s\n", channel);

    if (strpbrk(channel, "\x07\x2C"))
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else if (channel_len - 1 > CHANNELSTRSIZE)
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else if ((channel[0] != '#' && channel[0] != '&') || !is_valid_chan(channel))
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else if (channel_len < 4)
    {
        irc_reply(e, cs, ERR_NOSUCHCHANNEL, channel);
    }
    else
        return (0);
    return (-1);
}

void irc_join(t_env *e, int cs, t_token *tokens)
{
    char concat[CHANNELSTRSIZE + NICKNAMESTRSIZE + 11];

    if ((irc_join_check_command(e, cs, tokens)) != 0)
        return;

    memset(concat, 0, sizeof(concat));

    time2iso(e->isotime);

    sprintf(concat, "%s leaved %s.\n", e->fds[cs].nickname, e->fds[cs].channel);
    broadcast(e, concat, IRC_INFO, cs);

    printf(
        "\x1b[31m"
        "[%s]:"
        "\x1b[0m"
        " %s leaved %s\n",
        e->isotime, e->fds[cs].nickname, e->fds[cs].channel);

    memset(e->fds[cs].channel, 0, CHANNELSTRSIZE + 1);
    strncpy(e->fds[cs].channel, tokens[1].addr, tokens[1].len - 1);
    memset(concat, 0, sizeof(concat));

    sprintf(concat, "%s joined %s.\n", e->fds[cs].nickname, e->fds[cs].channel);
    broadcast(e, concat, IRC_INFO, cs);

    printf(
        "\x1b[31m"
        "[%s]:"
        "\x1b[0m"
        " %s joined %s\n",
        e->isotime, e->fds[cs].nickname, e->fds[cs].channel);
}
