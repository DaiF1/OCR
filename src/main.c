/*
 * main.c in src/
 * 
 * Made by julie.fiadino
 *
 * OCR main file
 *
 * Started on  06/10 julie.fiadino
 * Last Update 27/10 julie.fiadino
*/
#include "utils.h"
#include "interface.h"
#include "imgprocess.h"

gpointer thread_progress(gpointer user_data)
{
    Progress *progress = user_data;
    
    gtk_dialog_run(progress->dialog);

    return NULL;
}

void file_set(GtkFileChooserButton *button, gpointer user_data)
{
    UI *ui = user_data;

    gchar *filename = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(button));

    gtk_image_clear(ui->s_image);
    gtk_image_set_from_file(ui->s_image, filename);
}

void on_solve(GtkToolButton *button, gpointer user_data)
{
    UI *ui = user_data;


    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkDialog *dialog = GTK_DIALOG(gtk_message_dialog_new(ui->window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Solving Sudoku..."));

    GtkProgressBar *progress = GTK_PROGRESS_BAR(gtk_progress_bar_new());
    gtk_widget_set_parent_window(GTK_WIDGET(progress), GDK_WINDOW(dialog));
    gtk_widget_show(GTK_WIDGET(progress));
 
    Progress bar = {dialog, progress};

    g_thread_new("dialog", thread_progress, &bar);

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(ui->s_image);
    pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

    t_image img = {0};
    img.pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
    img.width = (int32)gdk_pixbuf_get_width(pixbuf);
    img.height = (int32)gdk_pixbuf_get_height(pixbuf);

    g_print("image collected!\n");

    gray_scale(&img);
    adjust_image(&img, 5);
    black_and_white(&img);

    g_print("image converted!\n");

    DEBUG_display_image(&img);

    gtk_widget_destroy(GTK_WIDGET(dialog));
    gtk_widget_destroy(GTK_WIDGET(progress));
}

void on_step(GtkToolButton *button, gpointer user_data)
{
    // TODO
}

void on_save(GtkToolButton *button, gpointer user_data)
{
    // TODO
}

void on_quit(GtkToolButton *button, gpointer user_data)
{
    // TODO
}

int main()
{
    gtk_init(NULL, NULL);

    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (gtk_builder_add_from_file(builder, "ocr.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkWindow *window =
        GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.ocr"));
    GtkImage *sudoku_image =
        GTK_IMAGE(gtk_builder_get_object(builder, "_Sudoku"));
    GtkFileChooserButton *open_button =
        GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "_Open"));
    GtkToolButton *save_button =
        GTK_TOOL_BUTTON(gtk_builder_get_object(builder, "_Save"));
    GtkToolButton *solve_button =
        GTK_TOOL_BUTTON(gtk_builder_get_object(builder, "_Solve"));
    GtkToolButton *step_button =
        GTK_TOOL_BUTTON(gtk_builder_get_object(builder, "_Solve_step"));
    GtkToolButton *quit_button =
        GTK_TOOL_BUTTON(gtk_builder_get_object(builder, "_Quit"));

    UI ui = {
        .window = window,
        .s_image = sudoku_image,
        .file_chooser = open_button,
        .save_button = save_button,
        .solve_button = solve_button,
        .step_button = step_button,
        .quit_button = quit_button
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(open_button, "file-set", G_CALLBACK(file_set), &ui);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve), &ui);
    g_signal_connect(step_button, "clicked", G_CALLBACK(on_step), &ui);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save), &ui);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit), &ui);


    gtk_main();

    return 0;
}
