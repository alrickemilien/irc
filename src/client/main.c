#include <arpa/inet.h>
#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/panel.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void init_env(t_env *e)
{
    size_t        i;
    struct rlimit rlp;

    // RLIMIT_NOFILE:
    // This specifies a value one greater than the maximum file
    // descriptor number that can be opened by this process.
    XSAFE(-1, getrlimit(RLIMIT_NOFILE, &rlp), "init_env::getrlimit");

    // there are three standard file descriptions, STDIN, STDOUT, and STDERR.
    // They are assigned to 0, 1, and 2 respectively.
    e->maxfd = rlp.rlim_cur;
    e->fds = (t_fd *)XPSAFE(NULL, malloc(sizeof(*e->fds) * e->maxfd),
                            "init_env::malloc");

    // Server's socket connection
    e->sock = -1;
    e->ipv6 = 0;

    XPSAFE(NULL, getcwd(e->cwd, sizeof(e->cwd)), "init_env::getcwd");

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
    stdin_fd->write = (void *)0;
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

int gui(t_env *e, int argc, char **argv)
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    printf("A\n");

    // When host has already been set through command line
    printf("e->sock: %d\n", e->sock);

    if (!e->options.host[0])
        window = login_window(e);

    printf("B\n");

    printf("e->sock 2: %d\n", e->sock);
    printf("e->options.host: %s\n", e->options.host);

    if (e->options.host[0] && _c2s_connect(e, NULL, NULL, e->options.host) < 0)
    {

    printf("0\n");
        window = login_window(e);

    }
    else if (e->sock != -1)
    {
        printf("e->sock 2: %d\n", e->sock);

        window = panel_window(e);
    }

    printf("C\n");

    gtk_widget_show_all(window);

    printf("D\n");

    // while (1)
    //     do_select(e);

    gtk_main();

    printf("E\n");

    return (0);
}

int main(int argc, char **argv)
{
    int            exit_code;
    t_env          e;
    struct timeval timeout;

    memset(&e, 0, sizeof(t_env));

    timeout.tv_sec = 0;
    timeout.tv_usec = 5000;

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
        loginfo("options.command: %s\n", e.options.command);

    init_std(&e);

    execute_precommands(&e);

    (void)timeout;
    while (1)
        do_select(&e);
    return (exit_code);
}
