#include <arpa/inet.h>
#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int init_env(t_env *e)
{
    size_t        i;
    struct rlimit rlp;

    // RLIMIT_NOFILE:
    // This specifies a value one greater than the maximum file
    // descriptor number that can be opened by this process.
    if (getrlimit(RLIMIT_NOFILE, &rlp) < 0)
        return (logerrno("init_env::getrlimit"));

    // there are three standard file descriptions, STDIN, STDOUT, and STDERR.
    // They are assigned to 0, 1, and 2 respectively.
    e->maxfd = rlp.rlim_cur;
    if ((e->fds = (t_fd *)malloc(sizeof(*e->fds) * e->maxfd)) == (void*)0)
        return (logerror("init_env::malloc"));

    // Server's socket connection
    e->sock = -1;

    if (getcwd(e->cwd, sizeof(e->cwd)) == (void *)0)
        return (logerrno("init_env::getcwd"));

    i = 0;
    while (i < e->maxfd)
    {
        memset(&e->fds[i], 0, sizeof(t_fd));
        clear_fd(&e->fds[i]);
        i++;
    }

    return (0);
}

static void init_options(t_options *options)
{
    // Set default port
    if (options->port == 0)
        options->port = 5555;

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

int main(int argc, char **argv)
{
    int   exit_code;
    t_env e;

    memset(&e, 0, sizeof(t_env));

    exit_code = read_options(argc, (const char **)argv, &e.options);
    if (exit_code != 0)
        return (exit_code);

    init_options(&e.options);

    e.argv_0 = argv[0];
    init_env(&e);

    if (e.options.ipv6 == 1)
        e.ipv6 = 1;

    if (e.options.gui)
        return (gui(&e, argc, argv));

    if (e.options.command[0])
        loginfo("options.command: %s", e.options.command);

    init_std(&e);

    execute_precommands(&e);

    while (1)
        do_select(&e);
    return (exit_code);
}
