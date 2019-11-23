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

#endif
