#pragma once

#include <stdint.h>

// Define the histogram structure
typedef struct hgram_t {
    uint32_t* bins;      // Array of bins (frequencies)
    size_t n_bins;       // Number of bins
    float min_value;     // Minimum value (left bound)
    float max_value;     // Maximum value (right bound)
    float bin_width;     // Width of each bin
} hgram_t;

/**
 * Initializes a histogram with the specified bin width and value range.
 *
 * This function allocates memory for a histogram (`hgram_t`), calculates the
 * number of bins required to cover the specified range from `min_value` to
 * `max_value` using the provided `bin_width`, and initializes the bins to zero.
 *
 * @param bin_width The width of each bin in the histogram.
 * @param min_value The minimum value for the histogram's range.
 * @param max_value The maximum value for the histogram's range.
 *
 * @return A pointer to the initialized `hgram_t` structure, representing the histogram.
 */
hgram_t* hgram_init(float bin_width, float min_value, float max_value);


/**
 * Adds a value to the histogram.
 *
 * This function increments the bin count for the bin corresponding to the given
 * value. If the value is outside the range specified during initialization,
 * it is ignored.
 *
 * @param hgram Pointer to the histogram (`hgram_t`) to which the value will be added.
 * @param value The value to be added to the histogram.
 */
void hgram_add(hgram_t* hgram, float value);

/**
 * Retrieves the count of the peak bin in the histogram.
 *
 * This function scans the histogram's bins and returns the count of the bin
 * with the highest value (the peak). The first and last bins are only compared
 * with their immediate neighbors, while all intermediate bins are compared
 * with both neighbors.
 *
 * @param hgram Pointer to the histogram (`hgram_t`) from which the peak bin count will be determined.
 *
 * @return The count of the bin with the highest value in the histogram.
 */
unsigned int hgram_peak_count(hgram_t* hgram);

/**
 * Frees the memory allocated for the histogram.
 *
 * This function frees the memory allocated for the histogram structure (`hgram_t`)
 * and the memory allocated for its bins.
 *
 * @param hgram Pointer to the histogram (`hgram_t`) that needs to be freed.
 */
void free_hgram(hgram_t* hgram);
