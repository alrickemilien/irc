#include <client/irc.h>

char *gtk_get_assets(
	const char *bin_path, const char *path)
{
	const char *bin_folder_path = extract_folder_from_path(
			bin_path);

	if (!bin_folder_path)
		return (NULL);
	return (strjoin(bin_folder_path, path));
}
