#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "pam_auth.h"

typedef struct AppWidgets {
    GtkWidget *window;
    GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *status_label;
    GtkWidget *login_button;
} AppWidgets;

static void set_status(AppWidgets *app, const char *message, gboolean is_error) {
    gtk_label_set_text(GTK_LABEL(app->status_label), message ? message : "");
    GdkRGBA color;
    if (is_error) {
        gdk_rgba_parse(&color, "#cc0000"); // red
    } else {
        gdk_rgba_parse(&color, "#008800"); // green
    }
    gtk_widget_override_color(app->status_label, GTK_STATE_FLAG_NORMAL, &color);
}

static void try_login(AppWidgets *app) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(app->password_entry));

    if (username == NULL || strlen(username) == 0 || password == NULL || strlen(password) == 0) {
        set_status(app, "Vui lòng nhập username và password.", TRUE);
        return;
    }

    gtk_widget_set_sensitive(app->login_button, FALSE);
    set_status(app, "Đang xác thực...", FALSE);

    char *error_message = NULL;
    gboolean ok = pam_authenticate_user(username, password, &error_message);

    if (ok) {
        set_status(app, "Đăng nhập thành công!", FALSE);
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(app->window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Xin chào %s", username);
        g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);
        g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_main_quit), NULL);
        gtk_widget_show_all(dialog);
    } else {
        if (error_message) {
            set_status(app, error_message, TRUE);
            free(error_message);
        } else {
            set_status(app, "Đăng nhập thất bại.", TRUE);
        }
        gtk_widget_set_sensitive(app->login_button, TRUE);
    }
}

static void on_login_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    AppWidgets *app = (AppWidgets *)user_data;
    try_login(app);
}

static void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    (void)entry;
    AppWidgets *app = (AppWidgets *)user_data;
    try_login(app);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    AppWidgets app = {0};

    app.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app.window), "Ubuntu Login (C + PAM + GTK)");
    gtk_window_set_default_size(GTK_WINDOW(app.window), 380, 220);
    gtk_container_set_border_width(GTK_CONTAINER(app.window), 16);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_container_add(GTK_CONTAINER(app.window), grid);

    GtkWidget *user_label = gtk_label_new("Username:");
    app.username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app.username_entry), "ví dụ: ubuntu");

    GtkWidget *pass_label = gtk_label_new("Password:");
    app.password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app.password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(app.password_entry), 0x25CF); // '●'

    app.login_button = gtk_button_new_with_label("Đăng nhập");
    app.status_label = gtk_label_new("");
    gtk_widget_set_halign(app.status_label, GTK_ALIGN_START);

    gtk_grid_attach(GTK_GRID(grid), user_label,         0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app.username_entry, 1, 0, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), pass_label,         0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app.password_entry, 1, 1, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), app.login_button,   1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app.status_label,   0, 3, 3, 1);

    g_signal_connect(app.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(app.login_button, "clicked", G_CALLBACK(on_login_clicked), &app);
    g_signal_connect(app.username_entry, "activate", G_CALLBACK(on_entry_activate), &app);
    g_signal_connect(app.password_entry, "activate", G_CALLBACK(on_entry_activate), &app);

    gtk_widget_show_all(app.window);
    gtk_main();

    return 0;
}
