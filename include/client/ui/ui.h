#ifndef CLIENT_UI_H
#define CLIENT_UI_H

#include <gtk/gtk.h>

GtkBuilder *builder;
GtkBuilder *mainwindow;
GtkWidget * window;
GtkWidget * usernameEntry;
GtkWidget * passwordEntry;
GtkWidget * button;
GtkWidget * label;
GtkWidget * eventBox;

GtkWidget *table;
GtkWidget *image;

GtkWidget *    textview1;
GtkWidget *    textview2;
GtkTextBuffer *textbuffer;

#endif