#pragma once

// forward declaration

typedef struct sgram_t sgram_t;

typedef struct cmap_t {
	unsigned int** freqs; // [samples]
	unsigned int** times; // [samples]
	size_t n_stacks_seg;
	size_t n_stacks_freq;
	double max_freq_Hz;
	double max_seg_sec;	
	double delta_time_sec;	// distance between audio samples, mult. float** times to get seconds
	double delta_freq_Hz;   // distance between freq samples, mult. float** freqs to get Hz
} cmap_t;

/**
 * Initializes and allocates memory for a constellation map (cmap) structure.
 * The cmap structure is used to store frequency-time coordinates and corresponding peak values
 * for segments of a spectrogram.
 *
 * @param n_stacks_freq Number of frequency stacks (number of windows in the frequency domain).
 * @param n_stacks_seg Number of segment stacks (number of windows in the time domain).
 *
 * @return A pointer to the allocated cmap structure.
 */
cmap_t* init_cmap(size_t n_stacks_freq, size_t n_stacks_seg);

/**
 * Generates a constellation map (cmap) from a given spectrogram. The cmap consists of
 * stacked windows in both the frequency and time domains, with peak values identified
 * for each window in the spectrogram.
 *
 * The cmap is created based on the provided frequency resolution (`delta_freq_Hz`)
 * and time resolution (`delta_seg_sec`). The function uses a sliding window approach
 * to identify peak values in the spectrogram and stores the frequency and time coordinates
 * of the peaks in the cmap structure.
 *
 * @param sgram The spectrogram structure from which the cmap is generated.
 * @param delta_freq_Hz The frequency resolution of the cmap (spacing between frequency bins in Hz).
 * @param delta_seg_sec The time resolution of the cmap (spacing between time bins in seconds).
 *
 * @return A pointer to the created cmap structure containing frequency-time coordinates of peaks.
 */
cmap_t* make_cmap(sgram_t* sgram, double delta_freq_Hz, double delta_seg_sec);

