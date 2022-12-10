#include <stdio.h>
#include <string.h>
#include "digit_network.h"
#include "convert.h"
#include "result_network.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <train/test> <filename>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "train") == 0)
    {
        if (argc != 2)
        {
            printf("Usage: %s <train/test> <(test) filename>\n", argv[0]);
            return 1;
        }

        train();
        return 0;
    }
    else if (strcmp(argv[1], "test") == 0)
    {
        if (argc != 3)
        {
            printf("Usage: %s <train/test> <(test) filename>\n", argv[0]);
            return 1;
        }

        Image image = SDL_Surface_to_Image(load_image(argv[2]));

        int res = neural_network_execute(&image);

        free_Image(&image);
        printf("%i",res);
    }
    else
    {
        printf("Usage: %s <save/train> <filename>\n", argv[0]);
        return 1;
    }
}
