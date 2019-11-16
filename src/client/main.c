#include <arpa/inet.h>
#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/ui/ui.h"
#include "client/ui/login.h"

t_env *    e = NULL;
t_options *options = NULL;

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

    XPSAFE(NULL, getcwd(e->cwd, sizeof(e->cwd)), "init_env::getcwd");

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

// static void execute_precommands(t_options *options, t_env *e)
// {
//     char *ptr;

//     ptr = options->command;
//     while (ptr && *ptr)
//     {
//         irc_command(e, e->sock, ptr);

//         ptr = strstr(ptr, "\x0D\x0A");

//         if (ptr)
//             ptr += 2;
//     }
// }

int main(int argc, char **argv)
{
    int exit_code;

    printf("%s\n",  argv[0]);
 
    e = malloc(sizeof(t_env));
    options = malloc(sizeof(t_options));

    exit_code = read_options(argc, (const char **)argv, options);
    if (exit_code != 0)
        return (exit_code);

    init_options(options);
    init_env(e);

    gtk_init(&argc, &argv);

    login_window_init();

    g_object_unref(G_OBJECT(builder));

    gtk_widget_show_all(window);

    gtk_main();

    return (exit_code);
}
