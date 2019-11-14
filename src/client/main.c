#include <arpa/inet.h>
#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void init_env(t_env *e)
{
    size_t i;

    // there are three standard file descriptions, STDIN, STDOUT, and STDERR.
    // They are assigned to 0, 1, and 2 respectively.
    // The last is used for client to server
    e->maxfd = 4;
    e->fds = (t_fd *)XPSAFE(NULL, malloc(sizeof(*e->fds) * e->maxfd),
                            "init_env::malloc");
    e->sock = -1;

    i = 0;
    while (i < e->maxfd)
    {
        clear_fd(&e->fds[i]);
        memset(e->fds[i].buf_write, 0, BUF_SIZE + 1);
        memset(e->fds[i].buf_read.data, 0, BUF_SIZE + 1);
        e->fds[i].buf_read.size = 0;
        i++;
    }
}

static void init_options(t_options *options)
{
    // Set default port
    if (options->port == 0)
        options->port = 5555;

    // Set default backlog
    if (options->host[0] == 0)
        memcpy(options->host, "127.0.0.1", sizeof(char) * 9);

    if (options->ipv6)
    {
        printf("Running server ipv6\n");
    }
}

static void execute_precommands(t_options *options, t_env *e)
{
    char *ptr;

    ptr = options->command;
    while (ptr && *ptr)
    {
        irc_command(e, e->sock, ptr);

        ptr = strstr(ptr, "\x0D\x0A");

        if (ptr)
            ptr += 2;
    }
}

int main(int argc, const char **argv)
{
    t_options options;
    int       exit_code;
    t_env     e;

    exit_code = read_options(argc, argv, &options);
    if (exit_code != 0)
        return (exit_code);

    init_options(&options);
    init_env(&e);

    if (options.ipv6 == 1)
        client_ipv6(&options, &e);
    else
        client_ipv4(&options, &e);

    if (e.sock == -1)
        return (logerrno("main:"));

    loginfo("options.command: %s\n", options.command);

    memset(e.fds[e.sock].nickname, 0, NICKNAMESTRSIZE + 1);
    memset(e.fds[e.sock].username, 0, USERNAMESTRSIZE + 1);
    memset(e.fds[e.sock].channelname, 0, CHANNELSTRSIZE + 1);

    execute_precommands(&options, &e);

    do_select(&options, &e);

    if (e.sock != -1)
        XSAFE(-1, close(e.sock), "main::close");

    return (exit_code);
}
