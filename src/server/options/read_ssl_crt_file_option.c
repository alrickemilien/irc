#include <server/irc.h>

int read_ssl_crt_file_option(t_options *options, const char *value)
{
    memcpy(options->ssl_crt_file, value, strlen(value));

    if (access(options->ssl_crt_file, R_OK) < 0)
        return (fprintf(
            stderr, "for the --ssl-crt-file option: cannot stat file '%s'.\n",
            value));
    return (0);
}
