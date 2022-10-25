/*
 * main.c in src/
 *
 * Made by julie.fiadino
 *
 * OCR main file
 *
 * Started on  21/10 nicolas.dek
 * Last Update 23/10 nicolas.dek
 */
#include <err.h>
#include <time.h> // POUR LA FUNCTION "DEBUG_color_component"
#include "loader.h"
#include "morpho.h"

#define max(a, b) ((a) > (b)) ? (a) : (b)

void merge_component_neighbour(int *pixels_label, int i, int j, int w, int h)
{
    int radius = 1;
    if (*(pixels_label+(i*w+j)) != 0)
    {
        int lowest_label = *(pixels_label+(i*w+j));
        for (int a=-radius; a<=radius; a++)
        {
            for (int b=-radius; b<=radius; b++)
            {
                if (i+a >= 0 && i+a < h && j+b >= 0 && j+b < w)
                {
                    if (pixels_label[(i+a)*w+(j+b)] < lowest_label && pixels_label[(i+a)*w+(j+b)] != 0)
                        lowest_label = pixels_label[(i+a)*w+(j+b)];
                }
            }
        }

        /* pixels_label[i*h+j] = lowest_label; */
        for (int a=-radius; a<radius; a++)
        {
            for (int b=-radius; b<radius; b++)
            {
                if (i+a >= 0 && i+a < h && j+b >= 0 && j+b < w)
                {
                    if (pixels_label[(i+a)*w+(j+b)] != 0 && pixels_label[(i+a)*w+(j+b)] != lowest_label)
                    {
                        pixels_label[(i+a)*w+(j+b)] = lowest_label;
                        merge_component_neighbour(pixels_label, i+a, j+b, w, h);
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
    int *pixels_label = calloc(sizeof(int), w*h);
    int left, top = 0;
    int tmp_label = 0;

    if (pixels_label == NULL)
        errx(1, "Not enough memorhihi!");

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            current_pixel = img->pixels+(i*w+j);
            current_color = (uint8) (*current_pixel) / 255.0;
            current_label = (pixels_label+(i*w+j));
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
                        /* merge_component_neighbour(pixels_label, i, j, w, h); */

                        if (pixels_label[top] < pixels_label[left]
                                && pixels_label[top] != 0)
                            tmp_label = pixels_label[top];
                        else
                            tmp_label = pixels_label[left];

                        pixels_label[top] = tmp_label;
                        pixels_label[left] = tmp_label;
                        *current_label = tmp_label;
                    }

                }

                // gere les erreurs de sortie de matrice
                if (j > 0 && *current_label == 0)
                {
                    // verifie la couleur du pixel de gauche
                    // pour prendre son label
                    if ((uint8) (img->pixels[left] / 255) == color_to_find)
                        *current_label = pixels_label[left];
                    // verifie si le pixel du haut et actuel ont la meme couleur et
                }

                // gere les erreurs de sortie de matrice
                if (i > 0 && *current_label == 0)
                {
                    // verifie la couleur du pixel de haut
                    // pour prendre son label
                    if ((uint8) (img->pixels[top] / 255) == color_to_find)
                        *current_label = pixels_label[top];

                }

                // on verifie si la couleur du haut et de gauche est blanc
                // c'est le dernier cas, normalement y'a plus rien a comparer
                if (*current_label == 0)
                {
                    /* if ((uint8) (img->pixels[top] / 255) == 0 || (uint8) (img->pixels[left] / 255) == 0) */
                    /*     printf("%u, %u, %d, %d\n", (uint8) (img->pixels[top] / 255), (uint8) (img->pixels[left] / 255), i, j); */
                    *current_label = ++latest_label;
                }

            }
        }
    }

    printf("%d\n", latest_label);
    // merge neighbours label
    for (int i=0; i < h; i++)
        for (int j=0; j < w; j++)
            merge_component_neighbour(pixels_label, i, j, w, h);
    return pixels_label;
}

int get_nb_component(int *component, int size)
{
    int latest_component = 0;
    for (int i = 0; i < size; i++)
    {
        if (component[i] > latest_component)
            latest_component = component[i];
    }
    return latest_component+1;
}

uint32 *get_size_component(int *component, int size)
{
    int nb_component = get_nb_component(component, size);
    uint32 *size_component = calloc(sizeof(uint32), nb_component+1);
    for (int i = 0; i < size; i++)
    {
        /* printf("%d, %d, %d, %d\n", nb_component, component[i], i, size); */
        if (component[i] >= 0)
            size_component[component[i]]++; 
    }
    return size_component;
}

int get_max_component(uint32 *size_component, int *component, int size)
{
    int max_component = 1;
    int nb_component = get_nb_component(component, size);
    for (int i=2; i < nb_component; i++)
    {
        if (size_component[max_component] < size_component[i])
            max_component = i;
    }
    return max_component;
    
}

#if DEBUG
void DEBUG_color_component(int *component, t_image *img, int label, uint32 color)
{
    for (int i=0; i < img->width*img->height; i++)
        if (*(component+i) == label)
            *(img->pixels+i) = color;
}
#endif

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
