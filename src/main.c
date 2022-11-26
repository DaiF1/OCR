/*
 * main.c in src/
 * 
 * Made by julie.fiadino
 *
 * OCR main file
 *
 * Started on  06/10 julie.fiadino
 * Last Update 21/10 julie.fiadino
*/
#include <gtk/gtk.h>

#include "utils.h"

typedef struct UI
{
    GtkWindow               *window;
    GtkImage                *s_image;
    GtkFileChooserButton    *file_chooser;
    GtkToolButton           *save_button;
    GtkToolButton           *solve_button;
    GtkToolButton           *step_button;
    GtkToolButton           *quit_button;
} UI;

void file_set(GtkFileChooserButton *button, gpointer user_data)
{
    UI *ui = user_data;

    gchar *filename = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(button));

    gtk_image_clear(ui->s_image);
    gtk_image_set_from_file(ui->s_image, filename);
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

    gtk_main();

    return 0;
}
