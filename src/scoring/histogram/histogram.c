#include "scoring/histogram/histogram.h"
#include <stdlib.h>
#include <assert.h>

// Function to initialize the histogram
hgram_t* init_hgram(float bin_width, float min_value, float max_value) {
    hgram_t* hgram = (hgram_t*)malloc(sizeof(hgram_t));
    assert(hgram != NULL);

    hgram->n_bins = (max_value - min_value) / bin_width;
    hgram->min_value = min_value;
    hgram->max_value = max_value;
    hgram->bin_width = (max_value - min_value) / hgram->n_bins;
    hgram->bins = (uint32_t*)calloc(hgram->n_bins, sizeof(uint32_t));  // Initialize bins to 0

    return hgram;
}

// Function to add a value to the histogram
void hgram_add(hgram_t* hgram, float value) {
    if (value < hgram->min_value || value >= hgram->max_value) {
        return; // Ignore values out of range
    }
    size_t bin_index = (size_t)((value - hgram->min_value) / hgram->bin_width);
    hgram->bins[bin_index]++;
}

unsigned int hgram_peak_count(hgram_t* hgram) {

    unsigned int peak_count = hgram->bins[0];

    // Check the bins from the second to the second-to-last (normal case, compare with both neighbors)
    for (unsigned int bin = 1; bin < hgram->n_bins - 1; bin++) {
        if (hgram->bins[bin] > peak_count) {
            peak_count = hgram->bins[bin];
        }
    }

    return peak_count;
}

// Function to free the allocated memory for the histogram
void free_hgram(hgram_t* hgram) {
    free(hgram->bins);
    free(hgram);
}