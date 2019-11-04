#include <netdb.h>
#include "irc.h"

int				read_host_option(
		t_options *options, const char *value)
{
	if (value[0] == 0)
		return (fprintf(stderr,
			"for the --host option: host must be a valid ip adresse: '%s'.\n", value));
    memcpy(options->host, value, sizeof(char) * strlen(value));
    if (gethostbyname(options->host) == (void*)0)
    	return (fprintf(stderr,
			"for the --host option: host must be a valid ip adresse': %s'.\n", value));
	return (0);	
}
