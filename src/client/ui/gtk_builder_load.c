#include <client/irc.h>
#include <client/ui/ui.h>

int gtk_builder_load(GtkBuilder *b,
                     const char *bin_path,
                     const char *ui_path)
{
    char *bin_folder_path;
    char *template_path;

    bin_folder_path = extract_folder_from_path(bin_path);

    if (!bin_folder_path)
        return (-1);

    template_path = strjoin(bin_folder_path, ui_path);

    if (!template_path)
        return (-1);

    printf("loading: %s\n", template_path);
    printf("bin_path: %s\n", bin_path);
    printf("ui_path: %s\n", ui_path);

    /* get graphics from login.glade */
    gtk_builder_add_from_file(b, template_path, (void *)0);

    free(bin_folder_path);
    free(template_path);

    return (0);
}
