#include <irc.h>

int	ato32(const char *str, uint32_t *nbr)
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
