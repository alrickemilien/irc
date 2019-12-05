#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

#include <client/irc.h>

int read_host_option(t_options *options, const char *value)
{
    struct addrinfo  hints;
    struct addrinfo *res;

    if (value[0] == 0)
        return (fprintf(
            stderr,
            "for the --host option: host must be a valid ip adress: '%s'.\n",
            value));

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(value, NULL, &hints, &res) != 0)
        return (fprintf(stderr,
                        "for the --host option: host must be a valid ip "
                        "adress: '%s'.\n",
                        value));

    while (res)
    {
        switch (res->ai_family)
        {
            case AF_INET:
                break;
            case AF_INET6:
                options->ipv6 = true;
                break;
            default:
                return (fprintf(
                    stderr,
                    "for the --host option: host must be a valid ipv4/v6 "
                    "adress: '%s'.\n",
                    value));
        }

        res = res->ai_next;
    }

    strcpy(options->host, value);

    return (0);
}
