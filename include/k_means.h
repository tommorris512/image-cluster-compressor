#ifndef K_MEANS_H
#define K_MEANS_H

/*
 * Define a typed struct to encapsulate KMeans models.
 */
typedef struct {
    double** centroids;
    int k;
    int num_variables;
} KMeans;

/* FUNCTION PROTOTYPES */

/*
 * Creates a new KMeans for a specified number of clusters and features.
 * Returns a pointer to a new KMeans on success and NULL on failure.
 */
KMeans* create_k_means(int k, int num_variables, double initial_centroid_range);

/*
 * Fits the KMeans model to a series of data samples.
 * This redistributes the centroids of the model based on the data samples given.
 */
void fit_k_means(KMeans* km, double** X, int num_samples, int num_iterations);

/*
 * Predicts the cluster of a given data point.
 * Returns the predicted cluster number based on the model's centroids.
 */
int predict_k_means(KMeans* km, double* X);

/*
 * Frees the dynamically allocated memory used by the KMeans model.
 */
void free_k_means(KMeans* km);

#endif /* For K_MEANS_H */
