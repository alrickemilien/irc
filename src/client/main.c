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

    // Server's socket connection
    e->sock = -1;
    e->ipv6 = 0;

    i = 0;
    while (i < e->maxfd)
    {
        clear_fd(&e->fds[i]);
        memset(&e->fds[i].nickname, 0, NICKNAMESTRSIZE + 1);
        memset(&e->fds[i].username, 0, USERNAMESTRSIZE + 1);
        memset(&e->fds[i].channelname, 0, CHANNELSTRSIZE + 1);
        cbuffer_reset(&e->fds[i].buf_read);
        cbuffer_reset(&e->fds[i].buf_write);
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
        loginfo("Running server ipv6\n");
}

static void init_std(t_env *e)
{
    t_fd *stdin_fd;
    // t_fd *stdout_fd;

    stdin_fd = &e->fds[0];
    stdin_fd->type = FD_CLIENT;
    stdin_fd->read = stdin_read;
}

static void execute_precommands(t_env *e)
{
    char *ptr;

    ptr = e->options.command;
    while (ptr && *ptr)
    {
        c2s(e, e->sock, ptr);

        ptr = strstr(ptr, "\x0D\x0A");

        if (ptr)
            ptr += 2;
    }
}

int main(int argc, const char **argv)
{
    int       exit_code;
    t_env     e;

    exit_code = read_options(argc, argv, &e.options);
    if (exit_code != 0)
        return (exit_code);

    init_options(&e.options);
    init_env(&e);

    if (e.options.ipv6 == 1)
        e.ipv6 = 1;

    if (e.options.command)
        loginfo("options.command: %s\n", e.options.command);

    execute_precommands(&e);

    init_std(&e);

    do_select(&e);

    if (e.sock != -1)
        XSAFE(-1, close(e.sock), "main::close");

    return (exit_code);
}
