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
#include <stddef.h>
#include "loader.h"
#include "utils.h"

#include "interface/interface.h"
#include "interface/output.h"

#include "imgprocess.h"
#include "sobel.h"
#include "morpho.h"
#include "textures.h"

#include "reader.h"
#include "solver.h"
#include "saver.h"

gpointer thread_progress(gpointer user_data)
{
    Progress *progress = user_data;
    
    gtk_dialog_run(progress->dialog);

    return NULL;
}

void dialog_error(GtkWindow *window, GtkMessageType type, char *msg)
{
    char *title = "Info";
    if (type == GTK_MESSAGE_ERROR)
        title = "Error";
    if (type == GTK_MESSAGE_WARNING)
        title = "Warning";

    GtkWidget *dialog;
    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_message_dialog_new_with_markup(window,
                                      flags,
                                      type,
                                      GTK_BUTTONS_CLOSE,
                                      "<b>%s</b>: %s", title, msg);

    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void file_set(GtkFileChooserButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File",
            interface->ui.window,
            action,
            "_Cancel",
            GTK_RESPONSE_CANCEL,
            "_Open",
            GTK_RESPONSE_ACCEPT,
            NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        
        gtk_image_clear(interface->ui.s_image);
        gtk_image_set_from_file(interface->ui.s_image, filename);

        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);
        pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

        int src_width = (int)gdk_pixbuf_get_width(pixbuf);
        int src_height = (int)gdk_pixbuf_get_height(pixbuf);

        pixbuf = gdk_pixbuf_scale_simple(pixbuf,
                src_width * 500 / src_height, 500,
                GDK_INTERP_BILINEAR);

        gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);

        interface->data.solved = false;

        g_free(filename);
    }

    gtk_widget_destroy (dialog); 
}

void on_solve(GtkModelButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    if (gtk_image_get_storage_type(interface->ui.s_image) ==
            GTK_IMAGE_EMPTY)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "No Image Given");
        return;
    }

    if (!interface->data.trained)
        dialog_error(interface->ui.window, GTK_MESSAGE_WARNING,
                "Neural Network not trained");
    else
        interface->data.solved = true;

    if (interface->data.solved)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_WARNING,
                "Grid already solved");
        return;
    }

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);

    t_image img = {0};
    img.pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
    img.width = (int32)gdk_pixbuf_get_width(pixbuf);
    img.height = (int32)gdk_pixbuf_get_height(pixbuf);

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

    int *labels = component_analysis(&copy);
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

    int grid[9][9] = {0};

    int grid_solved[9][9] = {0};

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            // format 'grid_xy.png'
            char *buffer = malloc(sizeof(char) * 18);
            sprintf(buffer, "boxes/grid_%i%i.png", x, y);
            save_and_crop_image(&copy, x * 24, y * 24, 24, 24, buffer);

            // TODO: number recognition
            int number = 0;

            grid[y][x] = -1;
            grid_solved[y][x] = number;
        }
    }

    system("rm -r boxes/");

    if (!solver(grid_solved, 0))
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "Unable to Solve Grid");
        return;
    }

    pixbuf = gdk_pixbuf_new_from_file("img/empty.png", NULL);
    pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

    int src_width = (int)gdk_pixbuf_get_width(pixbuf);
    int src_height = (int)gdk_pixbuf_get_height(pixbuf);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf,
            src_width * DEST_IMG_SIZE / src_height, DEST_IMG_SIZE, GDK_INTERP_BILINEAR);

    uint32 *pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);

    generate_output(grid, grid_solved, pixels);

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
}

void on_step(GtkModelButton *button, gpointer user_data)
{
    // TODO: Solve but show steps
}

void on_train(GtkModelButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    // TODO: Train the neural network

    interface->data.trained = true;
}

void on_load(GtkModelButton *button, gpointer user_data)
{
    // TODO: Load neural network weights
}

char* to_png(const char* filename)
{
    char* filename2;
    size_t len = 0;
    while (*(filename + len))
    {
        len++;
    }
    filename2 = malloc(sizeof(char)*(len+4) + 1);
    size_t i = 0;
    while (i < len)
    {
        *(filename2 + i) = *(filename + i);
        i++;
    }
    *(filename2 + i++) = '.';
    *(filename2 + i++) = 'p';
    *(filename2 + i++) = 'n';
    *(filename2 + i++) = 'g';
    return filename2;
}

void on_save(GtkModelButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    GtkWidget *dialog;
    GtkFileChooser *chooser;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                          interface->ui.window,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "Save",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    chooser = GTK_FILE_CHOOSER (dialog);

    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

        gtk_file_chooser_set_current_name (chooser,
                                           "Untitled_document");

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (chooser);
        //save_to_file (filename);
        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);
        pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
        gdk_pixbuf_save(pixbuf, to_png(filename), "png", NULL, NULL);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
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
    GtkModelButton *open_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Open"));
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

    Data data = {
        .angle = 0.0f,
        .trained = false,
        .solved = false,
    };

    Interface interface = {
        .ui = ui,
        .data = data,
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(open_button, "clicked", G_CALLBACK(file_set), &interface);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve), &interface);
    g_signal_connect(step_button, "clicked", G_CALLBACK(on_step), &interface);
    g_signal_connect(train_button, "clicked", G_CALLBACK(on_train), &interface);
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load), &interface);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save), &interface);


    gtk_main();

    return 0;
}
