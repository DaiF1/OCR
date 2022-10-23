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

int *component_analysis(t_image *img)
{
    // 0 = black
    // 1 = white
    uint8 color_to_find = 0;
    int h = img->height;
    int w = img->width;
    int latest_label = 1;
    int *current_label = NULL;
    uint32 *current_pixel = 0;
    uint8 current_color = 0;
    int *pixels_label = calloc(sizeof(int), w*h+ 1);
    int left, top = 0;


    if (pixels_label == NULL)
        errx(1, "Not enough memorhihi!");

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            current_pixel = img->pixels+(i*h+j);
            current_color = (uint8) (*current_pixel) / 255.0;
            current_label = (pixels_label+(i*h+j));
            left = (i*h)+(j-1);
            top  = ((i-1)*h)+j;
            

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
                        int tmp_label = latest_label;
                        if (pixels_label[top] <= pixels_label[left]
                                && pixels_label[top] != 0)
                            tmp_label = pixels_label[top];
                        else if (pixels_label[left] != 0)
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
                    *current_label = ++latest_label;

            }
        }
    }

    // merge neighbours label
    for (int i=0; i < img->height; i++)
    {
        for (int j=0; j < img->width; j++)
        {
            current_label = (pixels_label+(i*h+j));
            left = (i*h)+(j-1);
            top  = ((i-1)*h)+j;
            if (i > 0 && j > 0)
            {
                int tmp_label = *current_label;
                if (pixels_label[left] < pixels_label[top] && pixels_label[left] < *current_label)
                    tmp_label = pixels_label[left];
                else if (pixels_label[top] < pixels_label[left] && pixels_label[top] < *current_label)
                    tmp_label = pixels_label[top];
                *current_label = tmp_label;
                pixels_label[top] = tmp_label;
                pixels_label[left] = tmp_label;

            }
            else if (j > 0)
            {
                if (pixels_label[left] < *current_label)
                    *current_label = pixels_label[left];
                else
                    pixels_label[left] = *current_label;
            }
            else if (i > 0)
            {
                if (pixels_label[top] < *current_label)
                    *current_label = pixels_label[top];
                else
                    pixels_label[top] = *current_label;
            }
            

        }
    }
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

int *get_size_component(int *component, int size)
{
    int nb_component = get_nb_component(component, size);
    int *size_component = calloc(sizeof(int), nb_component+2);
    for (int i = 0; i < size; i++)
        size_component[component[i]]++; 
    return size_component;
}

void DEBUG_color_component(int *component, t_image *img)
{
    uint32 *current_color = 0;
    uint32 color[30] = {0};
    for (int i=0; i < 30; i++)
        color[i] = 0xFF000000;

    for (int i=0; i < img->width*img->height; i++)
    {
        if (*(component+i) != 0)
        {
            /* printf("%d\n", *(component+i)); */
            current_color = &color[*(component+i)];
            if (*current_color == 0xFF000000)
            {
                int result = 0;
                srand(time(NULL));
                result = (rand() % (16711680 - 0)) + 0;

                *current_color += result;
            }
            *(img->pixels+i) = *current_color;
        }
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
