#include <irc.h>

static const char *g_invalid_channel_char = "\x07\x20\x2C\x0A\x2C";

bool is_valid_chan_name(const char *channel, size_t len)
{
	size_t	i;

    if (channel[0] != '#' && channel[0] != '&')
        return (false);
    i = 1;
    while (channel[i] && channel[i] != '\x0D' && i < len)
    {
        if (strchr(g_invalid_channel_char, channel[i]))
            return (false);
        i++;
    }
    return (true);
}
