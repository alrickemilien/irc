#include <client/ui/panel.h>

int ui_set_status(t_ui_panel *ui, int status)
{
    ui->status_image =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "status_image"));

    switch (status)
    {
        case 0:
            gtk_image_set_from_file(GTK_IMAGE(ui->status_image),
                                    ui->status_ok_image);
            break;

        case 1:
            gtk_image_set_from_file(GTK_IMAGE(ui->status_image),
                                    ui->status_not_ok_image);
            break;
        case 2:
            gtk_image_set_from_file(GTK_IMAGE(ui->status_image),
                                    ui->status_away_image);
            break;
        default:
            break;
    }

    return (0);
}

int ui_nowaway(t_ui_panel *ui, const char *msg)
{
    (void)msg;
    ui_set_status(ui, 2);

    return (0);
}

int ui_unaway(t_ui_panel *ui)
{
    ui_set_status(ui, 0);

    return (0);
}
