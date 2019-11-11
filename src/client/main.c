#include <arpa/inet.h>
#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void init_env(t_env *e)
{
    e->fd.registered = 0;
    e->fd.type = FD_FREE;
    e->fd.read = NULL;
    e->fd.write = NULL;

    memset(e->fd.buf_write, 0, BUF_SIZE + 1);
    memset(e->fd.buf_read.data, 0, BUF_SIZE + 1);
    e->fd.buf_read.size = 0;
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

    XSAFE(-1, close(e.sock), "main::close");

    return (exit_code);
}
