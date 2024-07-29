#ifndef IMAGE_COMPRESSOR_H
#define IMAGE_COMPRESSOR_H


#include <stdint.h>

/* FUNCTION PROTOTYPES */

/*
 * Converts an image to a two-dimensional array of pixels.
 * Returns a two-dimensional double arrayp populated with numerical representations of colour.
 */
double** image_to_data(uint8_t* image, int width, int height, int channels);

/*
 * Converts a two-dimensional array of pixels back into a usable image format.
 * The image path provided is populated using the two-dimensional double array parsed.
 */
void data_to_image(double** data, uint8_t* image, int width, int height, int channels);

/*
 * Compresses a given input image to a specified output path for a given number of clusters and iteration count.
 * Returns a integer flag to determine success/failure.
 */
int compress_image(const char* input_image, const char* output_image, int num_clusters, int num_iterations);

#endif /* For IMAGE_COMPRESSOR_H*/
