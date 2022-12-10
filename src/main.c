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

#include "digit_network.h"
#include "convert.h"
#include "result_network.h"

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

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.png");
    gtk_file_filter_add_pattern(filter, "*.jpg");
    gtk_file_filter_add_pattern(filter, "*.bmp");
    gtk_file_filter_set_name(filter, "Images (.png/.jpg/.bmp)");

    GtkFileFilter *filter_grid = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_grid, "grid*");
    gtk_file_filter_set_name(filter_grid, "Grids");

    GtkFileFilter *filter_none = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_none, "*");
    gtk_file_filter_set_name(filter_none, "All Files");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_grid);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_none);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        GtkFileFilter *curr_filter = gtk_file_chooser_get_filter(chooser);

        if (strcmp(gtk_file_filter_get_name(curr_filter),
                    "Images (.png/.jpg/.bmp)") == 0)
        {
            gtk_image_clear(interface->ui.s_image);
            gtk_image_set_from_file(interface->ui.s_image, filename);

            GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);
            pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

            int src_width = (int)gdk_pixbuf_get_width(pixbuf);
            int src_height = (int)gdk_pixbuf_get_height(pixbuf);

            pixbuf = gdk_pixbuf_scale_simple(pixbuf,
                    src_width * MAX_IMG_SIZE / src_height, MAX_IMG_SIZE,
                    GDK_INTERP_BILINEAR);

            uint32 *pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
            interface->data.img.width = src_width * MAX_IMG_SIZE / src_height;
            interface->data.img.height = MAX_IMG_SIZE;

            interface->data.img.pixels = malloc(sizeof(uint32) *
                    interface->data.img.width * interface->data.img.height);
            memcpy(interface->data.img.pixels, pixels, 
                    sizeof(uint32) * interface->data.img.width *
                    interface->data.img.height);

            interface->data.processed = false;

            gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
        }
        else
        {
            gtk_image_clear(interface->ui.s_image);
            gtk_image_set_from_file(interface->ui.s_image, "img/empty.png");
            
            GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);
            pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
            pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

            int src_width = (int)gdk_pixbuf_get_width(pixbuf);
            int src_height = (int)gdk_pixbuf_get_height(pixbuf);

            pixbuf = gdk_pixbuf_scale_simple(pixbuf,
                    src_width * DEST_IMG_SIZE / src_height, DEST_IMG_SIZE, GDK_INTERP_BILINEAR);

            uint32 *pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
    
            read_sudoku(interface->data.grid, filename, 0);

            generate_output(interface->data.grid, interface->data.grid, pixels);

            interface->data.processed = true;

            gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
        }

        interface->data.solved = false;

        g_free(filename);
    }

    gtk_widget_destroy (dialog); 
}

void on_preproc(GtkModelButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    if (gtk_image_get_storage_type(interface->ui.s_image) ==
            GTK_IMAGE_EMPTY)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "No Image Given");
        return;
    }

    if (interface->data.processed)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_WARNING,
                "Grid already analysed");
        return;
    }

    if (!interface->data.trained)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "Neural Network not trained");
        return;
    }
    
    interface->data.processed = true;

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);

    t_image img = {0};
    img.pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
    img.width = (int32)gdk_pixbuf_get_width(pixbuf);
    img.height = (int32)gdk_pixbuf_get_height(pixbuf);

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

    get_corners(&copy, &copy);

    t_image result = {
        malloc(sizeof(uint32) * copy.width * copy.height),
        copy.width,
        copy.height
    };

    float angle = get_vertical_angle(copy);
    rotate(&copy, &result, angle);
    memcpy(copy.pixels, result.pixels,
            img.width * img.height * sizeof(uint32));

    rotate(&img, &result, angle);
    memcpy(img.pixels, result.pixels,
            img.width * img.height * sizeof(uint32));

    t_bounds bounds = {
        {-1, -1},
        {-1, -1},
        {-1, -1},
        {-1, -1}
    };

    for (int y = 0; y < copy.height; y++)
    {
        for (int x = 0; x < copy.width; x++)
        {
            if (copy.pixels[y * copy.width + x] == 0xffffffff)
                continue;

            float tl = mag(build((t_vector){0, 0},
                        (t_vector){x, y}));
            float tr = mag(build((t_vector){copy.width, 0},
                        (t_vector){x, y}));
            float bl = mag(build((t_vector){0, copy.height},
                        (t_vector){x, y}));
            float br = mag(build((t_vector){copy.width, copy.height},
                        (t_vector){x, y}));

            if (tl < mag(build((t_vector){0, 0}, bounds.tl)) || bounds.tl.x == -1)
                bounds.tl = (t_vector){x, y};
            if (tr < mag(build((t_vector){copy.width, 0}, bounds.tr)) || bounds.tr.x == -1)
                bounds.tr = (t_vector){x, y};
            if (bl < mag(build((t_vector){0, copy.height}, bounds.bl)) || bounds.bl.x == -1)
               bounds.bl = (t_vector){x, y};
            if (br < mag(build((t_vector){copy.width, copy.height}, bounds.br)) || bounds.br.x == -1)
                bounds.br = (t_vector){x, y};
        }
    } 

    result.pixels = realloc(result.pixels, sizeof(uint32)
            * DEST_IMG_SIZE * DEST_IMG_SIZE);
    result.width = DEST_IMG_SIZE;
    result.height = DEST_IMG_SIZE;

    remap(&img, &result, bounds);

    int answ = system("mkdir boxes");
    if (answ)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "Unable to generate tiles");
        return;
    }

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            // format 'grid_xy.png'
            char *buffer = malloc(sizeof(char) * 18);
            sprintf(buffer, "boxes/grid_%i%i.png", x, y);
            save_and_crop_image(&result, x * DEST_TILE_SIZE, y * DEST_TILE_SIZE,
                    DEST_TILE_SIZE, DEST_TILE_SIZE, buffer);

            Image image = SDL_Surface_to_Image(load_image(buffer));
            int number = result_network(&image,
                    interface->data.hw,
                    interface->data.hb,
                    interface->data.ow,
                    interface->data.ob);
            free_Image(&image);

            interface->data.grid[y][x] = (number == 0) ? -1 : number;
        }
    }

    answ = system("rm -r boxes/");
    if (answ)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_WARNING,
                "Unable to delete tiles");
    }

    pixbuf = gdk_pixbuf_new_from_file("img/empty.png", NULL);
    pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

    int src_width = (int)gdk_pixbuf_get_width(pixbuf);
    int src_height = (int)gdk_pixbuf_get_height(pixbuf);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf,
            src_width * DEST_IMG_SIZE / src_height, DEST_IMG_SIZE,
            GDK_INTERP_BILINEAR);

    uint32 *pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);

    generate_output(interface->data.grid, interface->data.grid, pixels);

    interface->data.processed = true;

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
}

void on_solve(GtkToolButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    if (interface->data.solved)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_WARNING,
                "Sudoku already solved");
        return;
    }

    int grid_solved[9][9] = {0};

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            grid_solved[y][x] = interface->data.grid[y][x];
        }
    }

    if (!solver(grid_solved, 0))
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "Unable to Solve Grid");
        return;
    }

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);

    pixbuf = gdk_pixbuf_new_from_file("img/empty.png", NULL);
    pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

    int src_width = (int)gdk_pixbuf_get_width(pixbuf);
    int src_height = (int)gdk_pixbuf_get_height(pixbuf);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf,
            src_width * DEST_IMG_SIZE / src_height, DEST_IMG_SIZE,
            GDK_INTERP_BILINEAR);

    uint32 *pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);

    generate_output(interface->data.grid, grid_solved, pixels);

    interface->data.solved = true;

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
}

void on_step(GtkModelButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    if (gtk_image_get_storage_type(interface->ui.s_image) ==
            GTK_IMAGE_EMPTY)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "No Image Given");
        return;
    }

    if (interface->data.processed)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_WARNING,
                "Grid already processed");
        return;
    }

    if (!interface->data.trained)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "Neural Network not trained");
        return;
    }
    
    interface->data.processed = true;

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);

    t_image img = {0};
    img.pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
    img.width = (int32)gdk_pixbuf_get_width(pixbuf);
    img.height = (int32)gdk_pixbuf_get_height(pixbuf);

    // TODO: rotation auto

    gray_scale(&img);

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Grayscale");

    adjust_image(&img, 2);

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Luminosity Adjusments");

    otsu(&img);

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Binarisation");

    t_image copy = {
        calloc(img.width * img.height, sizeof(uint32)),
        img.width,
        img.height
    };

    memcpy(copy.pixels, img.pixels,
            img.width * img.height * sizeof(uint32));

    int *labels = component_analysis(&img);
    int nb_labels = get_nb_of_labels(labels, img.height*img.width);
    int *size_of_labels = get_size_of_labels(labels, img.height*img.width);
    int max_label = get_max_label(size_of_labels, nb_labels);

    isolate_label(&img, labels, max_label);

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Grid Isolation");
    
    get_corners(&img, &img);

    t_image result = {
        malloc(sizeof(uint32) * copy.width * copy.height),
        copy.width,
        copy.height
    };

    float angle = get_vertical_angle(img);
    rotate(&img, &result, angle);
    memcpy(img.pixels, result.pixels,
            img.width * img.height * sizeof(uint32));

    rotate(&copy, &result, angle);
    memcpy(copy.pixels, result.pixels,
            img.width * img.height * sizeof(uint32));

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Image Rotation");

    t_bounds bounds = {
        {-1, -1},
        {-1, -1},
        {-1, -1},
        {-1, -1}
    };

    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {
            if (img.pixels[y * img.width + x] == 0xffffffff)
                continue;

            float tl = mag(build((t_vector){0, 0},
                        (t_vector){x, y}));
            float tr = mag(build((t_vector){img.width, 0},
                        (t_vector){x, y}));
            float bl = mag(build((t_vector){0, img.height},
                        (t_vector){x, y}));
            float br = mag(build((t_vector){img.width, img.height},
                        (t_vector){x, y}));

            if (tl < mag(build((t_vector){0, 0}, bounds.tl)) || bounds.tl.x == -1)
                bounds.tl = (t_vector){x, y};
            if (tr < mag(build((t_vector){img.width, 0}, bounds.tr)) || bounds.tr.x == -1)
                bounds.tr = (t_vector){x, y};
            if (bl < mag(build((t_vector){0, img.height}, bounds.bl)) || bounds.bl.x == -1)
                bounds.bl = (t_vector){x, y};
            if (br < mag(build((t_vector){img.width, img.height}, bounds.br)) || bounds.br.x == -1)
                bounds.br = (t_vector){x, y};
        }
    }

#if DEBUG
    DEBUG_draw_bounds(&img, bounds);
#endif
    
    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Grid Detection");

    result.pixels = realloc(result.pixels, sizeof(uint32)
            * DEST_IMG_SIZE * DEST_IMG_SIZE);
    result.width = DEST_IMG_SIZE;
    result.height = DEST_IMG_SIZE;

    remap(&copy, &result, bounds);

#if DEBUG
    DEBUG_display_image(&result);
#endif


    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Image Crop");

    int answ = system("mkdir boxes");
    if (answ)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_ERROR,
                "Unable to generate tiles");
        return;
    }

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            // format 'grid_xy.png'
            char *buffer = malloc(sizeof(char) * 18);
            sprintf(buffer, "boxes/grid_%i%i.png", x, y);
            save_and_crop_image(&result, x * DEST_TILE_SIZE, y * DEST_TILE_SIZE,
                    DEST_TILE_SIZE, DEST_TILE_SIZE, buffer);

            Image image = SDL_Surface_to_Image(load_image(buffer));
            int number = result_network(&image,
                    interface->data.hw,
                    interface->data.hb,
                    interface->data.ow,
                    interface->data.ob);
            free_Image(&image);

            
            interface->data.grid[y][x] = (number == 0) ? -1 : number;

        }
    }

    answ = system("rm -r boxes/");
    if (answ)
    {
        dialog_error(interface->ui.window, GTK_MESSAGE_WARNING,
                "Unable to delete tiles");
    }

    pixbuf = gdk_pixbuf_new_from_file("img/empty.png", NULL);
    pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);

    int src_width = (int)gdk_pixbuf_get_width(pixbuf);
    int src_height = (int)gdk_pixbuf_get_height(pixbuf);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf,
            src_width * DEST_IMG_SIZE / src_height, DEST_IMG_SIZE,
            GDK_INTERP_BILINEAR);

    uint32 *pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);

    generate_output(interface->data.grid, interface->data.grid, pixels);

    interface->data.processed = true;

    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);

    dialog_error(interface->ui.window, GTK_MESSAGE_OTHER,
                "Character Recognition");
}

void on_train(GtkModelButton *button, gpointer user_data)
{
    Interface *interface = user_data;

    train();
    load("save", interface->data.hw, interface->data.hb,
            interface->data.ow, interface->data.ob);

    interface->data.trained = true;
}

void on_load(GtkModelButton *button, gpointer user_data)
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

    GtkFileFilter *filter_save = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_save, "save*");
    gtk_file_filter_set_name(filter_save, "Save files");

    GtkFileFilter *filter_none = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_none, "*");
    gtk_file_filter_set_name(filter_none, "All Files");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_save);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_none);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        load(filename, interface->data.hw, interface->data.hb,
                interface->data.ow, interface->data.ob);

        g_free(filename);
    }

    gtk_widget_destroy (dialog); 

    interface->data.trained = true;
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
        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);
        pixbuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
        gdk_pixbuf_save(pixbuf, to_png(filename), "png", NULL, NULL);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

void on_rotate(GtkModelButton *button, gdouble v, gpointer user_data)
{
    v = v * 3.6;
    Interface *interface = user_data;
    if (v < 0)
    {
        v += 360;
    }
    interface->data.angle = v;

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->ui.s_image);
    t_image img = {0};
    img.pixels = (uint32 *)gdk_pixbuf_get_pixels(pixbuf);
    img.width = (int32)gdk_pixbuf_get_width(pixbuf);
    img.height = (int32)gdk_pixbuf_get_height(pixbuf);

    rotate(&interface->data.img, &img, v);
    gtk_image_set_from_pixbuf(interface->ui.s_image, pixbuf);
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
    GtkModelButton *preproc_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Preprocess"));
    GtkModelButton *step_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Step"));
    GtkModelButton *train_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Train"));
    GtkModelButton *load_button =
        GTK_MODEL_BUTTON(gtk_builder_get_object(builder, "_Load"));
    GtkScaleButton *rotate_button =
        GTK_SCALE_BUTTON(gtk_builder_get_object(builder, "_Rotate1"));
    GtkToolButton *solve_button =
        GTK_TOOL_BUTTON(gtk_builder_get_object(builder, "_Solve"));

    UI ui = {
        .window = window,
        .s_image = sudoku_image,
        .file_chooser = open_button,
        .save_button = save_button,
        .preproc_button = preproc_button,
        .step_button = step_button,
        .rotate_button = rotate_button,
        .solve_button = solve_button,
    };

    Data data = {
        .img = {0},
        .angle = 0.0f,
        .trained = false,
        .solved = false,
        .grid = {},

        .hw = calloc(1, sizeof(Matrix)),
        .hb = calloc(1, sizeof(Matrix)),
        .ow = calloc(1, sizeof(Matrix)),
        .ob = calloc(1, sizeof(Matrix)),
    };

    Interface interface = {
        .ui = ui,
        .data = data,
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(open_button, "clicked", G_CALLBACK(file_set), &interface);
    g_signal_connect(preproc_button, "clicked", G_CALLBACK(on_preproc), &interface);
    g_signal_connect(step_button, "clicked", G_CALLBACK(on_step), &interface);
    g_signal_connect(train_button, "clicked", G_CALLBACK(on_train), &interface);
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load), &interface);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save), &interface);
    g_signal_connect(rotate_button, "value-changed", G_CALLBACK(on_rotate), &interface);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve), &interface);

    gtk_main();

    return 0;
}
