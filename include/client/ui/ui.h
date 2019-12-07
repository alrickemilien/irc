#ifndef CLIENT_UI_H
#define CLIENT_UI_H

#include <gtk/gtk.h>
#include <client/irc.h>

int             gtk_builder_load(
    GtkBuilder *b, const char *bin_path, const char *ui_path);

int             gtk_provider_load_css(GtkCssProvider *p,
                          const char *    bin_path,
                          const char *    ui_path);
                          
void            gtk_set_class(GtkWidget *w, const char *str);
void            gtk_set_transparent_window(GtkWidget *w, GdkRGBA *c);
GdkRGBA         *gtk_new_rgba(float r, float g, float b, float a);
gboolean        gtk_do_select(void *e);
char            *gtk_get_assets(const char *bin_path, const char *path);
void            gtk_set_cursor_style(GtkWidget *w, const char *cursor_type);

/*
** utils
*/

GtkWidget       *ui_util_new_hour_label(const char *fmt, const char *class);
GtkWidget       *ui_util_new_logo(const char *path, const char *class);
GtkWidget       *ui_util_new_chat_label(const char *msg, const char *class);

#endif
