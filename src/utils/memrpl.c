#include <irc.h>

void	*memrpl(
	char *dest, size_t dest_size,
	const char *src, size_t src_size)
{
	memset(dest, 0, dest_size);
	return (memcpy(dest, src, src_size));
}
