#ifndef CLIENT_UI_H
#define CLIENT_UI_H

#include <gtk/gtk.h>

GtkWidget * eventBox;

GtkWidget *table;
GtkWidget *image;

GtkWidget *    textview1;
GtkWidget *    textview2;
GtkTextBuffer *textbuffer;

int gtk_builder_load(GtkBuilder *b, const char *bin_path, const char *ui_path);

int gtk_provider_load_css(GtkCssProvider *p,
                          const char *    bin_path,
                          const char *    ui_path);
                          
void gtk_set_class(GtkWidget *w, const char *str);

void gtk_set_transparent_window(GtkWidget *w,
                                float      r,
                                float      g,
                                float      b,
                                float      a);
gboolean gtk_do_select(void *e);
char *gtk_get_assets(const char *bin_path, const char *path);

#endif
