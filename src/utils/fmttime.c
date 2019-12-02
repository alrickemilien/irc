#include <time.h>
#include <irc.h>

/*
** Get formated time
*/

int	fmttime(char* str, const char *fmt)
{
	time_t		timer;
	struct tm	*tm_info;

	memset(str, 0, ISOTIMESTRSIZE);
	time(&timer);
	tm_info = localtime(&timer);
	if (strftime(str, ISOTIMESTRSIZE, fmt, tm_info) == 0)
		return (-1);
	return (0);
}
