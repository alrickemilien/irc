#include <client/ui/ui.h>

gboolean    supports_alpha = FALSE;
static void screen_changed(GtkWidget *widget,
                           GdkScreen *old_screen,
                           gpointer   userdata)
{
    /* To check if the display supports alpha channels, get the visual */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    (void)old_screen;
    (void)userdata;

    if (!visual)
    {
        // printf("Your screen does not support alpha channels!\n");
        visual = gdk_screen_get_system_visual(screen);
        supports_alpha = FALSE;
    }
    else
    {
        // printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

    gtk_widget_set_visual(widget, visual);
}

static gboolean draw(GtkWidget *widget, cairo_t *cr, gpointer userdata)
{
    (void)widget;

    GdkRGBA *c;

    c = (GdkRGBA *)userdata;

    cairo_save(cr);

    if (supports_alpha)
    {
        cairo_set_source_rgba(cr, c->red, c->green, c->blue,
                              c->alpha); /* transparent */
    }
    else
    {
        cairo_set_source_rgb(cr, c->red, c->green, c->blue); /* opaque white */
    }

    /* draw the background */
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);

    cairo_restore(cr);

    return FALSE;
}

void gtk_set_transparent_window(GtkWidget *w, GdkRGBA *c)
{
    gtk_widget_set_app_paintable(w, TRUE);

    g_signal_connect(G_OBJECT(w), "draw", G_CALLBACK(draw), c);

    screen_changed(w, NULL, NULL);
}
