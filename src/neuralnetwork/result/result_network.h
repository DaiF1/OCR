#ifndef RESULT_NETWORK_H
#define RESULT_NETWORK_H

#include "../../../include/neuralnetwork/maths_extra.h"
#include "../../../include/neuralnetwork/matrix.h"

int result_network(
    Image *image, Matrix *hw, Matrix *hb, Matrix *ow, Matrix *ob);
int neural_network_execute(Image *image);

#endif
