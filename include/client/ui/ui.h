#ifndef CLIENT_UI_H
#define CLIENT_UI_H

#include <gtk/gtk.h>

GtkBuilder *builder;
GtkBuilder *mainwindow;
GtkWidget * window;
GtkWidget * eventBox;

GtkWidget *table;
GtkWidget *image;

GtkWidget *    textview1;
GtkWidget *    textview2;
GtkTextBuffer *textbuffer;

int gtk_builder_load(GtkBuilder *b,
                     const char *bin_path,
                     const char *ui_path);

#endif
