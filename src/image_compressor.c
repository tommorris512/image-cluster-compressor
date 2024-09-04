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
 * Helper function to convert a given image to a two-dimensional array of pixel colour values.
 * Determines the total number of pixels in the image and allocates an array to store them.
 * Each subarray of the pixel array is populated with the pixel's corresponding colour, represented as an array of its colour channels.
 * The value of each colour channel is cast to a double for compatability with the KMeans model.
 *
 * Returns a two-dimensional array of pixel colour values, or a NULL if dynamic allocation fails.
 */
static double** image_to_data(uint8_t* image, int width, int height, int channels) {
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
 * Helper function to convert a two-dimensional array of pixel colour values to a given image.
 * The image data array is populated with the corresponding colour values, where each double channel is cast to the uint8_t.
 * 
 * Returns void but directly modifies the parsed image data array.
 */
static void data_to_image(double** pixel_array, uint8_t* image, int width, int height, int channels) {
    // Determine the quantity of pixels according to the parsed dimensions.
    int num_pixels = width * height;

    // Allocate the colour channel values for each pixel into the image data array.
    for (int i = 0; i < num_pixels; i++) {
        for (int j = 0; j < channels; j++) {
            image[i * channels + j] = (uint8_t) pixel_array[i][j];
        }
    }
}

/*
 * Generates a compressed version of a given .jpg image.
 * Loads the input .jpg image using the stb_image library and converts the image data to a pixel-colour array.
 * Creates and trains a KMeans model with the given number of clusters for the given quantity of iterations.
 * Assigns each pixel in the image the colour of its corresponding centroid (the compressed colour).
 * Converts the compressed pixel-colour array back to image data and writes to the output path using the stb_image library.
 * Any dynamically allocated memory used is freed upon success or failure of the compression.
 * 
 * Returns a suitable exit code to indicate the success/failure of the image compression.
 */
int compress_image(const char* input_image, const char* output_image, int num_clusters, int num_iterations) {
    // Declare the image dimensions and channel counter and load the image.
    int width, height, channels;
    uint8_t* image = stbi_load(input_image, &width, &height, &channels, 0);

    // Return a NULL if allocation fails.
    if (image == NULL) {
        fprintf(stderr, "Error: Failed to load image %s\n", input_image);
        return EXIT_FAILURE;
    }

    // Determine the quantity of pixels and convert the image to a pixel-colour array.
    int num_pixels = width * height;
    double** pixel_array = image_to_data(image, width, height, channels);

    // Free relevant memory and return a NULL if allocation fails.
    if (pixel_array == NULL) {
        stbi_image_free(image);
        return EXIT_FAILURE;
    }

    // Create a new KMeans model to utilise in the compression.
    KMeans* km = create_k_means(num_clusters, channels, 255.0);

    // Free relevant memory and return a NULL if allocation fails.
    if (km == NULL) {
        fprintf(stderr, "Error: Failed to create KMeans model\n");

        for (int i = 0; i < num_pixels; i++) {
            free(pixel_array[i]);
        }

        free(pixel_array);
        stbi_image_free(image);

        return EXIT_FAILURE;
    }

    // Train the KMeans model on the image data for the parsed number of iterations.
    fit_k_means(km, pixel_array, num_pixels, num_iterations);

    // Predict the colour value of each pixel and assign it to the pixel-colour array.
    for (int i = 0; i < num_pixels; i++) {
        int cluster = predict_k_means(km, pixel_array[i]);

        for (int j = 0; j < channels; j++) {
            pixel_array[i][j] = km->centroids[cluster][j];
        }
    }

    // Convert the pixel-colour array to image data.
    data_to_image(pixel_array, image, width, height, channels);

    // Write the image data to the specified output path.
    stbi_write_jpg(output_image, width, height, channels, image, width * channels);

    // Free the dynamically allocated memory used.
    for (int i = 0; i < num_pixels; i++) {
        free(pixel_array[i]);
    }

    free(pixel_array);
    stbi_image_free(image);
    free_k_means(km);

    // Print the completion and exit with a success code.
    printf("Image compression complete. Output file has been saved as %s\n", output_image);
    return EXIT_SUCCESS;
}

/*
 * Main function to receive arguments and execute the image compression.
 * Checks the correct number of arguments have been parsed and prints the usage to stderr otherwise.
 * Calls the image compression function parsing the given command-line arguments.
 * 
 * Returns the exit code generated by the image compression function.
 */
int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_image> <output_image> <num_clusters> <num_iterations>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Assign the arguments parsed.
    const char* input_image = argv[1];
    const char* output_image = argv[2];
    int num_clusters = atoi(argv[3]);
    int num_iterations = atoi(argv[4]);

    // Execute the compression on the parsed image.
    return compress_image(input_image, output_image, num_clusters, num_iterations);
}
