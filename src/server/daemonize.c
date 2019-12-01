#include <fcntl.h>
#include <server/irc.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

static int write_pidfile(pid_t pid)
{
    int    fd;
    char   pidstr[100];
    size_t pidstr_len;

    if ((fd = open(PIDFILE, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG)) <
        0)
        return (logerrno("write_pidfile::open"));

    if ((pidstr_len = i64toa(pid, pidstr, 100, 10)) == (size_t)-1)
        return (logerror("write_pidfile::i64toa"));

    if (write(fd, pidstr, pidstr_len) < 0)
        return (logerrno("write_pidfile::write"));

    if (close(fd) < 0)
        return (logerrno("write_pidfile::close"));

    return (0);
}

// Shoudl follow
// http://0pointer.de/public/systemd-man/daemon.html#New-Style%20Daemons
int daemonize(void)
{
    pid_t pid;
    int   x;

    // check of pid file is available
    if (write_pidfile(0) < 0)
        return (-1);

    /* Fork off the parent process */
    if ((pid = fork()) < 0)
        return (logerrno("daemonize::fork"));

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(0);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        return (logerrno("daemonize::setsid"));

    /* Catch, ignore and handle signals */
    // TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    if ((pid = fork()) < 0)
        return (logerrno("daemonize::fork"));

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

    x = sysconf(_SC_OPEN_MAX);
    while (x >= 0)
        close(x--);

    /* Open the log file */
    openlog("irc_daemon", LOG_PID, LOG_DAEMON);

    return (0);
}