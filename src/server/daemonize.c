#include <server/irc.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#define PIDFILE "./ircserver.pid"

static void write_pidfile(pid_t pid)
{
    int    fd;
    char   pidstr[100];
    size_t pidstr_len;

    fd = XSAFE(-1,
               open(PIDFILE, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG),
               "write_pidfile::open");
    pidstr_len =
        XSAFE(-1, i64toa(pid, pidstr, 100, 10), "write_pidfile::i64toa");
    XSAFE(-1, write(fd, pidstr, pidstr_len), "write_pidfile::write");
    XSAFE(-1, close(fd), "write_pidfile::close");
}

// Shoudl follow
// http://0pointer.de/public/systemd-man/daemon.html#New-Style%20Daemons
void daemonize(void)
{
    pid_t pid;

    // check of pid file is available
    write_pidfile(0);

    /* Fork off the parent process */
    pid = XSAFE(-1, fork(), "daemonize::fork");

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(0);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        XSAFE(-1, setsid(), "daemonize::setsid");

    /* Catch, ignore and handle signals */
    // TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = XSAFE(-1, fork(), "daemonize::fork");

    /* Success: Let the parent terminate */
    if (pid > 0)
    {
        write_pidfile(pid);
        exit(0);
    }

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    {
        close(x);
    }

    /* Open the log file */
    openlog("firstdaemon", LOG_PID, LOG_DAEMON);
}