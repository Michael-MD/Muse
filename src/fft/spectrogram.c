#include "fft/spectrogram.h"
#include "fft/dft.h"
#include "audio/audio.h"
#include "debugging/asserts.h"
#include <stdlib.h>
#include <assert.h>

#include "debugging/asserts.h"

sgram_t* init_sgram(size_t n_freq, size_t n_seg) {

	// create and populate sgram metadata
	sgram_t* sgram = (sgram_t*)malloc(sizeof(sgram_t));
	muse_assert_valid_ptr(sgram, "invalid sgram memory allocation.");
	sgram->n_freq = n_freq;
	sgram->n_seg = n_seg;

	// allocate memory for spectrogram 2D array
	sgram->freqs = (float**)malloc(sizeof(float*) * n_seg);
	muse_assert_valid_ptr(sgram->freqs, "invalid sgram frequencies memory allocation.");

	for (size_t freq = 0; freq < n_seg; freq++) {
		sgram->freqs[freq] = (float*)malloc(sizeof(float) * n_freq);
		muse_assert_valid_ptr(sgram->freqs[freq], "invalid time segments memory allocation.");
	}

	return sgram;

}


void free_sgram(sgram_t* sgram) {
	// Check if sgram is not NULL before freeing memory
	if (sgram == NULL) {
		return;
	}

	// Free the memory allocated for each frequency segment row
	for (size_t seg = 0; seg < sgram->n_seg; seg++) {
		free(sgram->freqs[seg]);
	}

	// Free the array of pointers to rows
	free(sgram->freqs);

	// Finally, free the sgram structure itself
	free(sgram);
}


sgram_t* make_sgram(audio_t* audio, double window_sec, double hop_sec) {

	muse_assert_valid_ptr(audio, "audio passed to make_sgram NULL.");
	assert(audio->n_channels == 1);

	size_t hop_samples = hop_sec * audio->sample_rate;
	size_t window_samples = window_sec * audio->sample_rate;

	// ensure segment length is divisible by 2
	if (hop_samples % 2 != 0) hop_samples = max(--hop_samples, 1);;

	// calculate number of segments based on total frames and segment size
	size_t n_seg = (audio->frames - window_samples) / hop_samples;

	// calculate number of frequency bins (FFT output size)
	size_t n_freq = window_samples / 2 + 1;

	sgram_t* sgram = init_sgram(n_freq, n_seg);

	// set segment duration [s]
	sgram->delta_seg_sec = hop_sec;

	// set max frequency (Nyquist frequency)
	sgram->max_freq_Hz = audio->sample_rate / 2;

	// set duration in seconds
	sgram->max_seg_sec = audio->duration_sec;

	// calculate frequency resolution (spacing between bins)
	sgram->delta_freq_Hz = sgram->max_freq_Hz / (double)sgram->n_freq;

	// assign additional metadata
	sgram->n_seg = n_seg;
	sgram->n_freq = n_freq;
	sgram->delta_seg_samples = hop_samples;

	// perform dfts on track segments
	for (unsigned int seg = 0; seg < sgram->n_seg; seg++) {
		dft_r2abs2(audio->samples + seg * hop_samples, window_samples, sgram->freqs[seg]);
	}

	return sgram;
}
