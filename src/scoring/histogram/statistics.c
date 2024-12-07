#include "scoring/histogram/statistics.h"
#include "fft/hashing.h"
#include "scoring/histogram/histogram.h"
#include <assert.h>

float mean(hgram_t* hgram) {

	assert(hgram->n_bins != 0);

	float mu = 0;

	for (unsigned int bin = 0; bin < hgram->n_bins; bin++) {
		mu += hgram->bins[bin];
	}

	mu /= hgram->n_bins;

	return mu;
}

float variance(hgram_t* hgram) {
	
	assert(hgram->n_bins != 0);

	float var = 0;
	float mu = mean(hgram);

	for (unsigned int bin = 0; bin < hgram->n_bins; bin++) {
		var += (hgram->bins[bin] - mu) * (hgram->bins[bin] - mu);
	}

	var /= hgram->n_bins;

	return var;

}