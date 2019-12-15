#include <client/irc.h>
#include <client/ui/panel.h>

int ui_clear_panel_window(t_env *e, t_ui_panel *ui)
{
    (void)e;
    free(ui->window_color);

    // Destroy assets path allocated
    free(ui->status_ok_image);
    free(ui->status_not_ok_image);
    free(ui->status_away_image);
    free(ui->topic_image);
    free(ui->error_image);
    free(ui->info_image);
    free(ui->rpl_away_image);

    g_object_unref(G_OBJECT(ui->builder));

    free(ui);
    return (0);
}

static int ui_init_panel_assets(t_env *e, t_ui_panel *ui)
{
    ui->status_ok_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-ok-16.png");
    ui->status_not_ok_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-annuler-16.png");
    ui->status_away_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-mode-veille-16.png");
    ui->topic_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-grand-hashtag-40.png");
    ui->error_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-haute-priority-100.png");
    ui->info_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-info-carré-16.png");
    ui->rpl_away_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-dormir-16.png");
    ui->invite_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-importer-24.png");

    return (0);
}

static int ui_init_panel_window(t_ui_panel *ui)
{
    ui->window =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "window_panel"));
    gtk_widget_add_events(ui->window, GDK_KEY_PRESS_MASK);
    g_signal_connect(ui->window, "key_press_event", G_CALLBACK(on_keypress),
                     ui);
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    ui->window_color =
        gtk_new_rgba(163.0f / 255.0f, 88.0f / 255.0f, 136.0f / 255.0f, 0.96);
    if (ui->window_color != NULL)
        gtk_set_transparent_window(ui->window, ui->window_color);
    return (0);
}

static int ui_init_panel_chatbox(t_env *e, t_ui_panel *ui)
{
    ui->channels_count = 0;
    ui->channel_index = -1;

    memset(ui->channels, 0, sizeof(ui->channels));

    ui->channels_box =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "channels_box"));

    ui->chat_box_viewport =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box_viewport"));

    ui->chat_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_entry"));

    g_signal_connect(ui->chat_entry, "activate", G_CALLBACK(on_entry), e);

    // Init scroll window
    ui->scrollwin =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "scrollwin"));

    return (0);
}

int ui_init_panel(t_env *e, t_ui_panel *ui)
{
    GtkCssProvider *css;

    // Init builder
    ui->builder = gtk_builder_new();
    if (gtk_builder_load(ui->builder, e->argv_0, "/ui/panel/panel.glade") < 0)
        return (-1);

    // Init style css
    css = gtk_css_provider_new();
    if (gtk_provider_load_css(css, e->argv_0, "/ui/panel/panel.css") < 0)
        return (-1);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(css),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
 
    g_object_unref(G_OBJECT(css));

    ui_init_panel_window(ui);

    ui_init_panel_assets(e, ui);

    ui_init_panel_chatbox(e, ui);

    ui->e = e;

    gtk_widget_show_all(ui->window);

    return (0);
}
