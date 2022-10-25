/*
 * main.c in src/
 *
 * Made by julie.fiadino
 *
 * OCR main file
 *
 * Started on  21/10 nicolas.dek
 * Last Update 25/10 julie.fiadino
 */
#include <err.h>
#include <math.h>
#include <time.h> // POUR LA FUNCTION "DEBUG_color_label"

#include "loader.h"
#include "morpho.h"
#include "sobel.h"

#define PI 3.1415926535
#define min(a, b) ((a) < (b)) ? (a) : (b)
#define max(a, b) ((a) > (b)) ? (a) : (b)

// ______________ Projection to find a label use in fill_label(..)

bool _find_label_up(int *labels, int x, int y, int w, int id)
{
    for (int i=x; i >= 0; i--)
    {
        if (labels[i*w+y] == id)
            return true;
    }
    return false;
}

bool _find_label_right(int *labels, int x, int y, int w, int h, int id)
{
    for (int i=y; i <= h; i++)
    {
        if (labels[x*w+i] == id)
            return true;
    }
    return false;
}

bool _find_label_left(int *labels, int x, int y, int w, int id)
{
    for (int i=y; i >= 0; i--)
    {
        if (labels[x*w+i] == id)
            return true;
    }
    return false;
}

bool _find_label_down(int *labels, int x, int y, int w, int id)
{
    for (int i=x; i < w; i++)
    {
        if (labels[i*w+y] == id)
            return true;
    }
    return false;
}

// ________________________end

int *fill_label(int *label, int w, int h, int id)
{
    int *mask = calloc(sizeof(int), w*h);
    if (mask == NULL)
        errx(1, "Not enough memorhihi!");

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            // if the current label is not the wanted one
            // we use projection do determine if we are inside the wanted
            // label
            if (label[i*w+j] != id)
            {
                if (_find_label_left(label, i, j, w, id)
                        && _find_label_right(label, i, j, w, h, id)
                        && _find_label_up(label, i, j, w, id)
                        && _find_label_down(label, i, j, w, id))
                    mask[i*w+j] = 1; // Yes we are inside the label
            }
            else
                mask[i*w+j] = 1; // This is literally the label

        }
    }
   return mask; 
}

void isolate_label(t_image *img, int *labels, int id)
{
    int w = img->width;
    int h = img->height;

    for (int i=0; i < h; i++)
    {
        for (int j=0; j < w; j++)
        {
            // if the current label is not the wanted one we change the
            // pixel to white
            if (labels[i*w+j] != id)
            {
                img->pixels[i*w+j] = 0xFFFFFFFF;
            }
        }
    }
    
}

void remove_background(t_image *img, int *labels, int id)
{
    // we fill the label with a mask 
    int *mask = fill_label(labels, img->width, img->height, id); 
    // we isolate this mask from the image
    isolate_label(img, mask, 1);
    for (int i=0; i < img->height; i++)
    {
        for (int j=0; j < img->width; j++)
        {
            if (mask[i*img->width+j] == 0)
                img->pixels[i*img->width+j] = 0xFFFFFFFF;
        }
    }
    free(mask);
}

void merge_label_neighbour(int *labels, int i, int j, int w, int h)
{
    /* merge_label_neighbour(..)
     *
     * merge label around the current pixels at i and j (x and y) im sorry
     * >_<
     *
     *  param labels: matrix from component_analysis(..)
     *  param i: x position of the pixel to check
     *  param j: y position of the pixel to check
     *  param w: the width of labels
     *  param h: the height of label
    */

    // radius around the current position to determine if the neighbours 
    // have the lowest label

    int radius = 1;
    if (*(labels+(i*w+j)) != 0)
    {
        int lowest_label = *(labels+(i*w+j));
        for (int a=-radius; a<=radius; a++)
        {
            for (int b=-radius; b<=radius; b++)
            {
                if (i+a >= 0 && i+a < h && j+b >= 0 && j+b < w)
                {
                    if (labels[(i+a)*w+(j+b)] < lowest_label && labels[(i+a)*w+(j+b)] != 0)
                        lowest_label = labels[(i+a)*w+(j+b)];
                }
            }
        }

        for (int a=-radius; a<=radius; a++)
        {
            for (int b=-radius; b<=radius; b++)
            {
                if (i+a >= 0 && i+a < h && j+b >= 0 && j+b < w)
                {
                    if (labels[(i+a)*w+(j+b)] != 0 && labels[(i+a)*w+(j+b)] != lowest_label)
                    {
                        labels[(i+a)*w+(j+b)] = lowest_label;
                        merge_label_neighbour(labels, i+a, j+b, w, h);
                    }
                }
            }
        }
    }
}

int *component_analysis(t_image *img)
{
    // 0 = black
    // 1 = white
    uint8 color_to_find = 0;
    int h = img->height;
    int w = img->width;
    int latest_label = 0;
    int *current_label = NULL;
    uint32 *current_pixel = 0;
    uint8 current_color = 0;
    int *labels = calloc(sizeof(int), w*h);
    int left, top = 0;
    int tmp_label = 0;

    if (labels == NULL)
        errx(1, "Not enough memorhihi!");

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            current_pixel = img->pixels+(i*w+j);
            current_color = (uint8) (*current_pixel) / 255.0;
            current_label = (labels+(i*w+j));
            left = (i*w)+(j-1);
            top  = ((i-1)*w)+j;
            

            if (current_color == color_to_find)
            {
                // gere les erreurs de sortie de matrice
                if (i > 0 && j > 0)
                {
                    // verifie la couleur du pixel du gauche et du haut
                    if ((uint8) (img->pixels[left] / 255) == (uint8) (img->pixels[top] / 255)
                            && (uint8)(img->pixels[left]/255) == current_color)
                    {
                        // on prend le plus petit label du haut et de gauche et on
                        // l'assigne au 3 (gauche, current, haut)
                        /* merge_component_neighbour(labels, i, j, w, h); */

                        if (labels[top] < labels[left]
                                && labels[top] != 0)
                            tmp_label = labels[top];
                        else
                            tmp_label = labels[left];

                        labels[top] = tmp_label;
                        labels[left] = tmp_label;
                        *current_label = tmp_label;
                    }

                }

                // gere les erreurs de sortie de matrice
                if (j > 0 && *current_label == 0)
                {
                    // verifie la couleur du pixel de gauche
                    // pour prendre son label
                    if ((uint8) (img->pixels[left] / 255) == color_to_find)
                        *current_label = labels[left];
                    // verifie si le pixel du haut et actuel ont la meme couleur et
                }

                // gere les erreurs de sortie de matrice
                if (i > 0 && *current_label == 0)
                {
                    // verifie la couleur du pixel de haut
                    // pour prendre son label
                    if ((uint8) (img->pixels[top] / 255) == color_to_find)
                        *current_label = labels[top];

                }

                // on verifie si la couleur du haut et de gauche est blanc
                // c'est le dernier cas, normalement y'a plus rien a comparer
                if (*current_label == 0)
                    *current_label = ++latest_label;

            }
        }
    }

    // merge neighbours label
    for (int i=0; i < h; i++)
        for (int j=0; j < w; j++)
            merge_label_neighbour(labels, i, j, w, h);
    return labels;
}

int get_nb_of_labels(int *labels, int size)
{
    int latest_label = 0;
    for (int i = 0; i < size; i++)
    {
        if (labels[i] > latest_label)
            latest_label = labels[i];
    }
    return latest_label+1;
}

int *get_size_of_labels(int *labels, int size)
{
    int nb_labels = get_nb_of_labels(labels, size);
    int *size_of_labels = calloc(sizeof(int), nb_labels+1);
    if (size_of_labels == NULL)
        errx(1, "Not enough memorhihi!");
    for (int i = 0; i < size; i++)
    {
        if (labels[i] >= 0)
            size_of_labels[labels[i]]++; 
    }
    return size_of_labels;
}

int get_max_label(int *size_of_labels, int nb_label)
{
    int max_label = 1;
    for (int i=2; i < nb_label; i++)
    {
        if (size_of_labels[max_label] < size_of_labels[i])
            max_label = i;
    }
    return max_label;
    
}

#if DEBUG
void DEBUG_color_component(int *component, t_image *img, int label, uint32 color)
{
    for (int i=0; i < img->width*img->height; i++)
        if (*(component+i) == label)
            *(img->pixels+i) = color;
}
#endif

void get_corners(const t_image *src, t_image *dest)
{
    t_image dx = {
        calloc(src->width * src->height, sizeof(uint32)),
        src->width,
        src->height
    };
    sobel(src, &dx, 1, 0);

    t_image dy = {
        calloc(src->width * src->height, sizeof(uint32)),
        src->width,
        src->height
    };

    extract_hv(src, &dx, &dy);

    for (int i = 0; i < src->width * src->height; i++)
    {
        uint32 px = (dx.pixels[i] << 8) >> 8;
        uint32 py = (dy.pixels[i] << 8) >> 8;
        uint32 c = ((0xffffff - px) & (0xffffff - py));
        dest->pixels[i] = 0xffffffff + ((c << 8) >> 8);
    }
}

void gray_scale(t_image *img)
{
    for (int i = 0; i < img->width * img->height; i++)
    {

        float r = (float) ((uint8)(img->pixels[i] >> 16)) / 255.0;
        float g = (float) ((uint8)(img->pixels[i] >> 8)) / 255.0;
        float b = (float) ((uint8) img->pixels[i]) / 255.0;

        float m = max(max(r, g), b);

        img->pixels[i] = 0xff000000 +
            ((uint8)(m * 255.0) << 16) +
            ((uint8)(m * 255.0) << 8) +
            (uint8)(m * 255.0);
    }
}

void black_and_white(t_image *img)
{
    // !!! *img must already been a grayscale image !!!

    // will be the sum of all the grey pixel level divide by the number of pixel
    float treshold = 0;

    // calculate the treshold
    for (int i = 0; i <  img->width * img->height; i++)
        treshold += (float) ((uint8)(img->pixels[i] >> 16)) / 255.0;
    treshold /=  (img->width * img->height);

    // change every pixel based on the treshold
    for (int i = 0; i < img->width * img->height; i++)
    {
        // get the grey level of the current pixel
        float grey_level = (float)((uint8)(img->pixels[i] >> 16)) / 255.0;

        // binary_color contain black or white value
        uint32 binary_color = grey_level < treshold ? 0 : 255; 
        //apply
        img->pixels[i] = 0xff000000 + (binary_color << 16) +
            (binary_color << 8) + binary_color;
    }
}

void adjust_image(t_image *img, int8 precision)
{
    size_t len = 2 * precision + 1;
    int32 *ce = malloc(sizeof(int32) * len * len);
    circle_element(ce, precision);

    t_image closing = {
        malloc(sizeof(uint32) * img->width * img->height),
        img->width,
        img->height,
    };

    morpho_closing(img, &closing, ce, len);

    free(ce);

    for (size_t i = 0; i < (size_t)img->width * (size_t)img->height; i++)
    {
        float colsrc = (float)((uint8)img->pixels[i]) / 255.0;
        float colclosing = (float)((uint8)closing.pixels[i]) / 255.0;

        float newcol = colsrc / colclosing;
        newcol = (newcol > 1.0) ? 1.0 : newcol;
        uint8 newp = (uint8)(newcol * 255.0);

        img->pixels[i] = 0xff000000 + (newp << 16) + (newp << 8) + newp;
    }

    free(closing.pixels);
}

void rotate(t_image *src, t_image *dest, float angle)
{
    float r = (angle - 90) * PI / 180;

    float cs = cosf(r);
    float sn = sinf(r);

    int middle_x = src->width / 2;
    int middle_y = src->height / 2;

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            int u = x - middle_x;
            int v = y - middle_y;

            int cx = u * sn + v * cs + middle_x;
            int cy = u * cs - v * sn + middle_y;

            cx = max(min(cx, src->width), 0);
            cy = max(min(cy, src->height), 0);

            dest->pixels[y * src->width + x] = src->pixels[cy * src->width + (src->width - cx)];
        }
    }
}
