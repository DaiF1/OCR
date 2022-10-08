/*
 * test.c in tests/
 *
 * Made by julie.fiadino
 *
 * Main executable for unit testing
 *
 * Started on  06/10 julie.fiadino
 * Last Update 08/10 julie.fiadino
*/

#include <stdlib.h>
#include <assert.h>
#include "loader.h"


int main()
{
    t_image img = load_img("img/000.png");
    assert(img.pixels);
    return EXIT_SUCCESS; 
}
