#include <errno.h>
#include <irc.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static int is_tty = -1;

int		loginfo(const char *fmt, ...)
{
	va_list	ap;
	char	final_fmt[LOGSIZE + 1];
	char	t[ISOTIMESTRSIZE];

	if (is_tty == -1)
		is_tty = isatty(1);
	time2iso(t);
	sprintf(final_fmt, is_tty ? "[%s] "
			"\x1b[92m"
			"INFO:"
			"\x1b[0m"
			" %s\n"
			: "[%s] INFO: %s\n",
			t, fmt);
	va_start(ap, fmt);
	vprintf(final_fmt, ap);
	va_end(ap);
	return (0);
}

int		logerror(const char *fmt, ...)
{
	va_list	ap;
	char	final_fmt[LOGSIZE + 1];
	char	t[ISOTIMESTRSIZE];

	if (is_tty == -1)
		is_tty = isatty(1);
	time2iso(t);
	sprintf(final_fmt, is_tty ? "[%s] "
			"\x1b[31m"
			"ERROR:"
			"\x1b[0m"
			" %s\n"
			: "[%s] ERROR: %s\n",
			t, fmt);
	va_start(ap, fmt);
	vprintf(final_fmt, ap);
	va_end(ap);
	return (-1);
}

int		logdebug(const char *fmt, ...)
{
    va_list ap;
    char    final_fmt[LOGSIZE + 1];
    char    t[ISOTIMESTRSIZE];

    if (!DEBUG)
        return (0);

    if (is_tty == -1)
        is_tty = isatty(1);

    time2iso(t);
    sprintf(final_fmt, is_tty ? "[%s] "
                                "\x1b[33m"
                                "DEBUG:"
                                "\x1b[0m"
                                " %s\n"
                              : "[%s] DEBUG: %s\n",
            t, fmt);
    va_start(ap, fmt);
    vprintf(final_fmt, ap);
    va_end(ap);

    return (0);
}

int		logerrno(const char *str)
{
	char	t[ISOTIMESTRSIZE];

	if (is_tty == -1)
		is_tty = isatty(1);
	time2iso(t);
	printf(is_tty ? "[%s] "
			"\x1b[31m"
			"ERROR:"
			"\x1b[0m"
			" %s: %s\n"
			: "[%s] ERROR: %s: %s\n",
			t, str,
			errno | h_errno ? strerror(errno | h_errno) : "Unknown error");
	return (-1);
}
