#include "server/irc.h"

int				read_backlog_option(
		t_options *options, const char *value)
{
	if (value[0] == 0)
		return (-1);
	if (ato64(value, &options->backlog) != 0 || options->backlog < 1 || options->port > 99999)
		return (fprintf(stderr,
			"for the --backlog option: backlog must be a vakue between 1 an 99999'%s'.\n", value));
	return (0);	
}
