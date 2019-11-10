#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <client/irc.h>

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

    exit_code = read_options(argc, argv, &options);
    if (exit_code != 0)
        return (exit_code);

    init_options(&options);

    int sock = client_ipv4();

    close(sock);

    return (exit_code);
}
