#include "server/irc.h"

int		ato64(const char *str, uint64_t *nbr)
{
	*nbr = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'
			|| *str == '\v' || *str == '\r' || *str == '\f')
		str++;
	if (*str == '-')
		return (-1);
	else if (*str == '+')
		str++;
	while (*str && *str >= '0' && *str <= '9')
		*nbr = *nbr * 10 + *str++ - '0';
	return (0);
}
