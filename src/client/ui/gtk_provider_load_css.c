#include <client/irc.h>
#include <client/ui/ui.h>

int gtk_provider_load_css(GtkCssProvider *provider,
		const char *bin_path,
		const char *ui_path)
{
	char	*bin_folder_path;
	char	*template_path;
	int		ret;
	GError	*error;

	error = NULL;
	ret = 0;
	bin_folder_path = extract_folder_from_path(bin_path);
	if (!bin_folder_path)
		return (-1);
	template_path = strjoin(bin_folder_path, ui_path);
	if (!template_path)
		return (-1);
	gtk_css_provider_load_from_path(provider, template_path, &error);
	if (error != NULL)
	{
		logerror("Unable to read file :: %s\n", error->message);
		g_error_free(error);
		ret = -1;
	}
	free(bin_folder_path);
	free(template_path);
	return (ret);
}
