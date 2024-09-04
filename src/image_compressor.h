#include <stdint.h>

#ifndef IMAGE_COMPRESSOR_H
#define IMAGE_COMPRESSOR_H

/* FUNCTION PROTOTYPES */

/*
 * Compresses a given input image to a specified output path for a given number of clusters and iteration count.
 * Returns a integer flag to determine success/failure.
 */
int compress_image(const char* input_image, const char* output_image, int num_clusters, int num_iterations);

#endif /* For IMAGE_COMPRESSOR_H*/
