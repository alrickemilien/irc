#include <server/irc.h>
#include <signal.h>
#include <stdlib.h>

int init_env(t_env *e)
{
    size_t        i;
    struct rlimit rlp;

    // RLIMIT_NOFILE:
    // This specifies a value one greater than the maximum file
    // descriptor number that can be opened by this process.
    if (getrlimit(RLIMIT_NOFILE, &rlp) < 0)
        return (logerrno("init_env::getrlimit"));

    e->maxfd = rlp.rlim_cur;
    e->maxchannels = rlp.rlim_cur + 1;

    if ((e->fds = (t_fd *)malloc(sizeof(t_fd) * e->maxfd)) == (void *)0)
        return (logerrno("init_env::malloc"));

    // We need at least same number of channel as number of users
    if ((e->channels = (t_channel *)malloc(sizeof(t_channel) * e->maxfd)) ==
        (void *)0)
        return (logerrno("init_env::malloc"));

    i = 0;
    while (i < e->maxfd)
    {
        e->fds[i].ssl = NULL;
        clear_fd(&e->fds[i]);
        cbuffer_reset(&e->fds[i].buf_write);
        cbuffer_reset(&e->fds[i].buf_read);
        i++;
    }

    // clear channels
    memset(e->channels, 0, sizeof(t_channel) * e->maxchannels);
    memcpy(e->channels[0].channel, DEFAULT_CHANNEL, sizeof(DEFAULT_CHANNEL));

    return (0);
}

static void init_options(t_options *options)
{
    // Set default port
    if (options->port == 0)
        options->port = 5555;

    // Set default backlog
    if (options->backlog == 0)
        options->backlog = 42;

    if (options->daemon)
        daemonize();

    // Set default backlog
    if (options->bind[0] == 0)
        memcpy(options->bind, "127.0.0.1", sizeof(char) * 9);

    if (options->ipv6)
    {
        loginfo("Running server ipv6");
    }
}

int main(int argc, const char **argv)
{
    int       exit_code;
    t_options options;
    t_env     e;

    memset(&e, 0, sizeof(e));

    // signal(SIGPIPE, SIG_IGN);

    exit_code = read_options_arguments(argc, argv, &options);
    if (exit_code != 0)
        return (exit_code);

    init_options(&options);

    loginfo("Running at %s:%d", options.bind, options.port);

    init_env(&e);

    if (options.ipv6 == 1)
        server_ipv6(&options, &e);
    else
        server_ipv4(&options, &e);

    do_select(&e);

    return (exit_code);
}
