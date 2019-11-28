#include <arpa/inet.h>
#include <client/irc.h>
#include <client/ui/panel.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

static gboolean on_keypress(GtkWidget *  widget,
                            GdkEventKey *event,
                            gpointer     data)
{
    (void)widget;
    (void)data;

    if (event->keyval == GDK_KEY_Escape)
        gtk_main_quit();
    return FALSE;
}

static void chat_entry_send(GtkWidget *widget, gpointer data)
{
    const char *text;
    t_env *     e;

    e = (t_env *)data;

    text = gtk_entry_get_text(GTK_ENTRY(widget));

    printf("Entry contents: %s\n", text);

    c2s(e, e->sock, strdup(text));

    gtk_entry_set_text(GTK_ENTRY(widget), "");
}

// void on_full(GtkWidget *widget, GtkAllocation *allocation, void *data) {
//     printf("width = %d, height = %d\n", allocation->width,
//     allocation->height);
// }

// g_signal_connect(mywidget, "size-allocate", G_CALLBACK(my_getsize), NULL);

void scroll_to_bottom(t_ui_panel *ui)
{
    static gdouble last_upper = 0;

    GtkAdjustment *verticalAdjust;
    gdouble        adjust = 0;
    gdouble        lower = 0;
    gdouble        upper = 0;
    gdouble        page_size = 0;
    gdouble        step_size = 0.1;

    ui->scrollwin =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "scrollwin"));
    gtk_widget_show_all(ui->scrollwin);

    verticalAdjust =
        gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(ui->scrollwin));
    upper = gtk_adjustment_get_upper(verticalAdjust);

    if (last_upper == 0)
        last_upper = upper;
    else if (last_upper == upper)
        return;

    lower = gtk_adjustment_get_lower(verticalAdjust);
    upper = gtk_adjustment_get_upper(verticalAdjust);
    page_size = gtk_adjustment_get_page_size(verticalAdjust);
    step_size = ((upper - page_size) - lower) / page_size;

    logdebug("lower: %f\n", lower);
    logdebug("upper: %f\n", upper);
    logdebug("page_size: %f\n", page_size);
    logdebug("step_size: %f\n", step_size);
    logdebug("last_upper: %f\n", last_upper);

    adjust = upper - page_size - 1;

    logdebug("adjust: %f\n", adjust);
    logdebug("new adjust: %f\n",
             upper - page_size + (2 * upper - 2 * last_upper));

    gtk_adjustment_set_value(verticalAdjust,
                             upper - page_size + (upper - last_upper));
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(ui->scrollwin),
                                        verticalAdjust);

    last_upper = upper;
    gtk_widget_show_all(ui->scrollwin);
}

void new_chat_message(t_ui_panel *ui, const char *msg)
{
    GtkWidget *w;

    logdebug("ui::new_chat_message:: %s\n", msg);

    ui->chat_box = GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box"));

    ui->msg_count++;

    w = gtk_label_new(msg);
    gtk_set_class(w, "chat-message");
    gtk_label_set_xalign(GTK_LABEL(w), 0);
    gtk_widget_set_margin_start(GTK_WIDGET(w), 12);
    gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box), w, -1);
    gtk_widget_show_all(ui->chat_box);

    scroll_to_bottom(ui);
}

void set_channel_name(t_ui_panel *ui, const char *msg)
{
    logdebug("ui::set_channel_name:: %s\n", msg);

    ui->channel_label =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "channel_label"));

    gtk_label_set_text(GTK_LABEL(ui->channel_label), msg);
    gtk_widget_show_all(ui->channel_label);
}

void set_nick_name(t_ui_panel *ui, const char *msg)
{
    logdebug("ui::set_nick_name:: %s\n", msg);

    ui->nick_label =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "nickname_label"));

    gtk_label_set_text(GTK_LABEL(ui->nick_label), msg);
    gtk_widget_show_all(ui->nick_label);
}

void set_user_name(t_ui_panel *ui, const char *msg)
{
    logdebug("ui::set_user_name:: %s\n", msg);

    ui->user_label =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "username_label"));

    gtk_label_set_text(GTK_LABEL(ui->user_label), msg);
    gtk_widget_show_all(ui->user_label);
}

int set_status(t_ui_panel *ui, int status)
{
    ui->status_image =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "status_image"));

    printf("status_ok_image: %s\n", ui->status_ok_image);

    if (status == 0)
        gtk_image_set_from_file(GTK_IMAGE(ui->status_image),
                                ui->status_ok_image);

    if (status == 2)
        gtk_image_set_from_file(GTK_IMAGE(ui->status_image),
                                ui->status_away_image);

    // free(bin_folder_path);
    // free(template_path);

    return (0);
}

void ui_join_from_side_channel(GtkWidget *widget, gpointer data)
{
    const char *channel;
    t_ui_panel *ui;

    ui = (t_ui_panel*)data;

    channel = gtk_label_get_text(GTK_LABEL(widget));

    _c2s_join(&ui->e->fds[ui->e->sock], channel, strlen(channel));
}

int ui_join(t_ui_panel *ui, const char *channel)
{
    GtkWidget *w;

    logdebug("ui::ui_join:: %s\n", channel);

    set_channel_name(ui, channel);

    ui->channels_box =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "channels_box"));

    ui->channels_count++;

    w = gtk_label_new(channel);
    gtk_set_class(w, "channels-box-item");
    gtk_label_set_xalign(GTK_LABEL(w), 0);
    gtk_widget_set_margin_start(GTK_WIDGET(w), 12);
    g_signal_connect(w, "clicked", G_CALLBACK(ui_join_from_side_channel), ui);
    gtk_list_box_insert(GTK_LIST_BOX(ui->channels_box), w, -1);

    gtk_widget_show_all(ui->channels_box);

    return (0);
}

int ui_away(t_ui_panel *ui, const char *channel)
{
    logdebug("ui::ui_away:: %s\n", channel);

    set_status(ui, 2);

    return (0);
}

int ui_clear_panel_window(t_env *e, t_ui_panel *ui)
{
    (void)e;
    free(ui->window_color);
    g_object_unref(G_OBJECT(ui->builder));
    return (0);
}

int ui_init_panel_window(t_env *e, t_ui_panel *ui)
{
    GtkCssProvider *cssProvider;

    // Init builder
    ui->builder = gtk_builder_new();
    if (gtk_builder_load(ui->builder, e->argv_0, "/ui/panel/panel.glade") < 0)
        return (-1);

    // Init window
    ui->window =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "window_panel"));
    gtk_widget_add_events(ui->window, GDK_KEY_PRESS_MASK);
    g_signal_connect(ui->window, "key_press_event", G_CALLBACK(on_keypress),
                     NULL);
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Init style css
    cssProvider = gtk_css_provider_new();
    if (gtk_provider_load_css(cssProvider, e->argv_0, "/ui/panel/panel.css") <
        0)
        return (-1);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(G_OBJECT(cssProvider));

    ui->chat_box = GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box"));

    ui->msg_count = 0;

    new_chat_message(ui, "toto: msg number 1");
    new_chat_message(ui, "ayya: msg number 2");
    new_chat_message(ui, "ayya: msg number 2");
    new_chat_message(ui, "ayya: msg number 2");
    new_chat_message(ui, "ayya: msg number 2");
    new_chat_message(ui, "ayya: msg number 2");

    ui->chat_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_entry"));

    g_signal_connect(ui->chat_entry, "activate", G_CALLBACK(chat_entry_send),
                     e);

    ui->window_color = gtk_new_rgba(0.58431, 0.14902, 0.70196, 0.96);

    gtk_set_transparent_window(ui->window, ui->window_color);

    // Load assets
    ui->status_ok_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-ok-16.png");
    ui->status_not_ok_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-annuler-16.png");
  ui->status_away_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-mode-veille-16.png");

    gtk_widget_show_all(ui->window);

    return (0);
}
