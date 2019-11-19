#include <arpa/inet.h>
#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/ui/login.h"
#include "client/ui/ui.h"

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

    // Set default host to connect
    if (options->host[0] == 0)
        memcpy(options->host, "127.0.0.1", sizeof(char) * 9);

    if (options->ipv6)
        loginfo("%s\n", "Running server ipv6");
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
    gtk_init(&argc, &argv);

    login_window_init(e);

    g_object_unref(G_OBJECT(builder));

    gtk_widget_show_all(window);

    gtk_main();

    return (0);
}

int main(int argc, char **argv)
{
    int   exit_code;
    t_env e;

    exit_code = read_options(argc, (const char**)argv, &e.options);
    if (exit_code != 0)
        return (exit_code);

    init_options(&e.options);

    e.argv_0 = argv[0];
    init_env(&e);

    if (e.options.ipv6 == 1)
        e.ipv6 = 1;

    if (e.options.gui)
        return (gui(&e, argc, argv));

    if (e.options.command)
        loginfo("options.command: %s\n", e.options.command);

    init_std(&e);

    execute_precommands(&e);

    do_select(&e);

    return (exit_code);
}
