#include "fft/constellation.h"
#include "fft/spectrogram.h"
#include <assert.h>
#include <stdlib.h>
#include "debugging/asserts.h"

cmap_t* init_cmap(size_t n_stacks_freq, size_t n_stacks_seg) {

	// create and populate cmap metadata
	cmap_t* cmap = (sgram_t*)malloc(sizeof(cmap_t));
	muse_assert_valid_ptr(cmap, "invalid sgram memory allocation.");
	cmap->n_stacks_freq = n_stacks_freq;
	cmap->n_stacks_seg = n_stacks_seg;

	// allocate memory for constellation 2D array
	cmap->freqs = (unsigned int**)malloc(sizeof(unsigned int*) * n_stacks_seg);
	cmap->times = (unsigned int**)malloc(sizeof(unsigned int*) * n_stacks_seg);

	muse_assert_valid_ptr(cmap->freqs, "invalid cmap times memory allocation.");
	muse_assert_valid_ptr(cmap->times, "invalid cmap times memory allocation.");


	for (size_t freq = 0; freq < n_stacks_seg; freq++) {
		cmap->freqs[freq] = (unsigned int*)malloc(sizeof(unsigned int) * n_stacks_freq);
		cmap->times[freq] = (unsigned int*)malloc(sizeof(unsigned int) * n_stacks_freq);
		
		muse_assert_valid_ptr(cmap->freqs[freq], "invalid cmap frequencies memory allocation.");
		muse_assert_valid_ptr(cmap->times[freq], "invalid cmap frequencies memory allocation.");
	}

	return cmap;

}

static void find_peak_sgram_window(sgram_t* sgram, unsigned int delta_seg_samples, unsigned int delta_freq_samples, unsigned int seg_window, unsigned int freq_window, cmap_t* cmap) {

	float max_val = 0;

	for (unsigned int seg_offset = 0; seg_offset < delta_seg_samples; seg_offset++) {
		for (unsigned int freq_offset = 0; freq_offset < delta_freq_samples; freq_offset++) {

			size_t freq = freq_window * delta_freq_samples + freq_offset;
			size_t seg = seg_window * delta_seg_samples + seg_offset;

			float sgram_val = sgram->freqs[seg][freq];

			if (sgram_val > max_val) {
				max_val = sgram_val;
				cmap->freqs[seg_window][freq_window] = freq;
				cmap->times[seg_window][freq_window] = seg;
			}

		}
	}

}

cmap_t* make_cmap(sgram_t* sgram, double delta_freq_Hz, double delta_seg_sec, double max_freq_cutoff_Hz) {

	// update sgram metadata
	sgram->n_freq = (max_freq_cutoff_Hz / sgram->max_freq_Hz) * sgram->n_freq;
	sgram->max_freq_Hz = max_freq_cutoff_Hz;

	// delta_freq [Hz] -> delta_freq_Hz [samples] -- window height in sgram
	unsigned int delta_freq_samples = (delta_freq_Hz / sgram->max_freq_Hz) * sgram->n_freq;

	// delta_seg [s] -> delta_seg [samples] -- window width in sgram
	unsigned int delta_seg_samples = (delta_seg_sec / sgram->delta_seg_sec);

	// number of windows to be stacked vertically
	unsigned int n_stacks_freq = sgram->n_freq / delta_freq_samples;

	// number of windows to be stacked horizontally
	unsigned int n_stacks_seg = sgram->n_seg / delta_seg_samples;

	// allocate memory for peak coordinates in frequency-time
	cmap_t* cmap = init_cmap(n_stacks_freq, n_stacks_seg);

	// eventhough cmap has spacing of delta_freq_Hz and delta_seg_sec, the coordinates themselves
	// are still those of the sgram and therefore those are the values stored.
	cmap->delta_time_sec = sgram->delta_seg_sec;
	cmap->delta_freq_Hz = sgram->delta_freq_Hz;

	// set max frequency and corresponding track duration
	cmap->max_seg_sec = sgram->max_seg_sec;
	cmap->max_freq_Hz = sgram->max_freq_Hz;

	// loop through window stacks
	for (unsigned int seg_window = 0; seg_window < n_stacks_seg; seg_window++) {
		for (unsigned int freq_window = 0; freq_window < n_stacks_freq; freq_window++) {

			find_peak_sgram_window(sgram, delta_seg_samples, delta_freq_samples, seg_window, freq_window, cmap);

		}
	}

	return cmap;

}

