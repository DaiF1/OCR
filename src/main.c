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
#include <stdio.h>
#include "utils.h"
#include "interface/interface.h"
#include "interface/output.h"
#include "imgprocess.h"
#include "reader.h"
#include "solver.h"
#include "saver.h"
#include "sobel.h"
#include "morpho.h"

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

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(ui->s_image);
    pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

    int src_width = (int)gdk_pixbuf_get_width(pixbuf);
    int src_height = (int)gdk_pixbuf_get_height(pixbuf);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf,
            src_width * 500 / src_height, 500, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(ui->s_image, pixbuf);
}

void on_solve(GtkModelButton *button, gpointer user_data)
{
    UI *ui = user_data;

    if (gtk_image_get_storage_type(ui->s_image) ==
            GTK_IMAGE_EMPTY)
        return;

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(ui->s_image);

    t_image img = {0};
    img.pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
    img.width = (int32)gdk_pixbuf_get_width(pixbuf);
    img.height = (int32)gdk_pixbuf_get_height(pixbuf);

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

    // TODO: rotation auto

    gray_scale(&img);
    adjust_image(&img, 2);
    otsu(&img);

    t_image copy = {
        calloc(img.width * img.height, sizeof(uint32)),
        img.width,
        img.height
    };

    memcpy(copy.pixels, img.pixels,
            img.width * img.height * sizeof(uint32));

    int *labels = component_analysis(&img);
    int nb_labels = get_nb_of_labels(labels, copy.height*copy.width);
    int *size_of_labels = get_size_of_labels(labels, copy.height*copy.width);
    int max_label = get_max_label(size_of_labels, nb_labels);

    isolate_label(&copy, labels, max_label);

    // TODO: grid detection

    /*
    int32 *ce = malloc(sizeof(int32) * 25);
    circle_element(ce, 2);
    morpho_erosion(&copy, &img, ce, 2);
    get_corners(&img, &img);
    */

    
    // TODO: image crop

    system("mkdir boxes");

    int grid[9][9] = {
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    };

    int grid_solved[9][9] = {
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    };

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            // format 'grid_xy.png'
            char *buffer = malloc(sizeof(char) * 18);
            sprintf(buffer, "boxes/grid_%i%i.png", x, y);
            save_and_crop_image(&img, x * 24, y * 24, 24, 24, buffer);

            // TODO: number recognition
            int number = 0;

            grid[y][x] = number;
            grid_solved[y][x] = number;
        }
    }

    system("rm -r boxes/ && rmdir boxes");

    if (!solver(grid_solved, 0))
    {
        // TODO: Show message box with error
        return;
    }
    
    // TODO: write to image
    generate_output(grid, grid_solved, img.pixels);

    gtk_widget_destroy(GTK_WIDGET(dialog));
    gtk_widget_destroy(GTK_WIDGET(progress));
    gtk_image_set_from_pixbuf(ui->s_image, pixbuf);
}

void on_step(GtkModelButton *button, gpointer user_data)
{
    // TODO: Solve but show steps
}

void on_train(GtkModelButton *button, gpointer user_data)
{
    // TODO: Train the neural network
}

void on_load(GtkModelButton *button, gpointer user_data)
{
    // TODO: Load neural network weights
}

void on_save(GtkModelButton *button, gpointer user_data)
{
    UI *ui = user_data;
    printf("debut\n");

    GtkWidget *dialog;
    GtkFileChooser *chooser;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                          ui->window,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "Save",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    chooser = GTK_FILE_CHOOSER (dialog);

    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

        gtk_file_chooser_set_current_name (chooser,
                                           "Untitled document");

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (chooser);
        //save_to_file (filename);
        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(ui->s_image);
        pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
        gdk_pixbuf_save(pixbuf, filename, "png", NULL, NULL);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);

    /*
    GtkWidget* dialogue = gtk_file_chooser_dialog_new("Select a destination", ui->window, GTK_FILE_CHOOSER_ACTION_SAVE,
                                                      "test",
                                                      "Cancel", GTK_RESPONSE_CANCEL,
                                                      "Ok", GTK_RESPONSE_OK,
                                                      NULL);

    gtk_dialog_add_button(GTK_DIALOG(dialogue), "Cancel", GTK_RESPONSE_CANCEL);
    gtk_dialog_add_button(GTK_DIALOG(dialogue), "Ok", GTK_RESPONSE_OK);
    gchar *filename = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(button));
    printf("%s\n", filename);*/
    /*
    GtkWidget *dialog gtk_file_chooser_dialog_new
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(button), "");

    printf("eee\n");
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(ui->s_image);
    pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
    gdk_pixbuf_save(pixbuf, filename, "png", NULL, NULL);*/
    // TODO: Save image to file
    // NOTE: See pixbuf class for image saving utility
}

void on_rotate(GtkModelButton *button, gpointer user_data)
{
    // TODO: display dialog for rotation
}

void on_autorot(GtkModelButton *button, gpointer user_data)
{
    // TODO: automatic rotation
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
    GtkModelButton *save_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Save"));
    GtkModelButton *solve_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Solve"));
    GtkModelButton *step_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Step"));
    GtkModelButton *train_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Train"));
    GtkModelButton *load_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Load"));
    GtkModelButton *rotate_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Rotate"));
    GtkModelButton *autorot_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_AutoRot"));

    UI ui = {
        .window = window,
        .s_image = sudoku_image,
        .file_chooser = open_button,
        .save_button = save_button,
        .solve_button = solve_button,
        .step_button = step_button,
        .rotate_button = rotate_button,
        .autorot_button = autorot_button
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(open_button, "file-set", G_CALLBACK(file_set), &ui);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve), &ui);
    g_signal_connect(step_button, "clicked", G_CALLBACK(on_step), &ui);
    g_signal_connect(train_button, "clicked", G_CALLBACK(on_train), &ui);
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load), &ui);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save), &ui);


    gtk_main();

    return 0;
}
