#include <server/irc.h>

int read_ssl_key_file_option(t_options *options, const char *value)
{
    memcpy(options->ssl_key_file, value, strlen(value));

    if (access(options->ssl_key_file, R_OK) < 0)
        return (fprintf(
            stderr, "for the --ssl-key-file option: cannot stat file '%s'.\n",
            value));
    return (0);
}
