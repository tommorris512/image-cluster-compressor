/* Include relevant standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* Include k_means and stb_image libraries */
#include "k_means.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/* Include the relevant prototypes for the image compressor */
#include "image_compressor.h"

/*
 * Converts a given image to a two-dimensional array of pixel colour values.
 * Determines the total number of pixels in the image and allocates an array to store them.
 * Each subarray of the pixel array is populated with the pixel's corresponding colour, represented as an array of its colour channels.
 * The value of each colour channel is cast to a double for compatability with the KMeans model.
 *
 * Returns a two-dimensional array of pixel colour values, or a NULL if dynamic allocation fails.
 */
double** image_to_data(uint8_t* image, int width, int height, int channels) {
    // Dymanically allocate the pixel-colour array.
    int num_pixels = width * height;
    double** pixel_array = (double**) malloc(num_pixels * sizeof(double*));
    
    // Return a NULL if allocation fails.
    if (pixel_array == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for image data\n");
        return NULL;
    }

    for (int i = 0; i < num_pixels; i++) {
        // Allocate an array of colour channels for the current pixel.
        pixel_array[i] = (double*) malloc(channels * sizeof(double));

        // Free relevant memory and return a NULL if allocation fails.
        if (pixel_array[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for image data\n");

            for (int j = 0; j < i; j++) {
                free(pixel_array[j]);
            }

            free(pixel_array);
            return NULL;
        }

        // Populate each pixel's colour channel using the image data parsed.
        for (int j = 0; j < channels; j++) {
            pixel_array[i][j] = (double) image[i * channels + j];
        }
    }

    return pixel_array;
}

/*
 * Converts a two-dimensional array of pixel colour values to a given image.
 * The image data array is populated with the corresponding colour values, where each double channel is cast to the uint8_t.
 * 
 * Returns void but directly modifies the parsed image data array.
 */
void data_to_image(double** pixel_array, uint8_t* image, int width, int height, int channels) {
    // Determine the quantity of pixels according to the parsed dimensions.
    int num_pixels = width * height;

    // Allocate the colour channel values for each pixel into the image data array.
    for (int i = 0; i < num_pixels; i++) {
        for (int j = 0; j < channels; j++) {
            image[i * channels + j] = (uint8_t) pixel_array[i][j];
        }
    }
}
