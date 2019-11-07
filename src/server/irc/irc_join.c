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

static int irc_join_check_command(t_env *e, int cs, const char **command)
{
    printf("command[0]:%s\n", command[0]);

    if (!command[0] || !command[1] || command[2])
    {
        strcpy(e->fds[cs].buf_write,
               "\x1b[31mERROR\x1b[0m"
               " Usage: /join <#channel>\n");
    }
    else if (strlen(command[1]) - 1 > CHANNELSTRSIZE)
    {
        strcpy(e->fds[cs].buf_write,
               "\x1b[31mERROR\x1b[0m"
               " Channel name too long\n");
    }
    else if (command[1][0] != '#' || !is_valid_chan(command[1]))
    {
        strcpy(e->fds[cs].buf_write,
               "\x1b[31mERROR\x1b[0m"
               " Invalid character(s)\n");
    }
    else if (strlen(command[1]) < 4)
    {
        strcpy(e->fds[cs].buf_write,
               "\x1b[31mERROR\x1b[0m"
               " Channel name too short\n");
    }
    else
        return (0);
    return (-1);
}

void irc_join(t_env *e, int cs, char **command)
{
    char concat[CHANNELSTRSIZE + NICKNAMESTRSIZE + 11];

    if ((irc_join_check_command(e, cs, (const char**)command)) != 0)
        return;

    memset(concat, 0, sizeof(concat));

    time2iso(e->isotime);

    sprintf(concat, "%s leaved %s.\n", e->fds[cs].nickname, e->fds[cs].channel);
    broadcast(e, concat, IRC_INFO, cs);

    printf(
        "\xb1[31m"
        "[%s]"
        "\xb1[0m"
        " %s leaved %s\n",
        e->isotime, e->fds[cs].nickname, e->fds[cs].channel);

    memset(e->fds[cs].channel, 0, CHANNELSTRSIZE + 1);
    strncpy(e->fds[cs].channel, command[1], strlen(command[1]) - 1);
    memset(concat, 0, sizeof(concat));

    sprintf(concat, "%s joined %s.\n", e->fds[cs].nickname, e->fds[cs].channel);
    broadcast(e, concat, IRC_INFO, cs);

    printf(
        "\x1b[31m"
        "[%s]"
        "\x1b[0m"
        " %s joined %s\n",
        e->isotime, e->fds[cs].nickname, e->fds[cs].channel);
}
