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
 * Creates a Coordinate Map (cmap) from a spectrogram, finding peak coordinates
 * within specified frequency and time windows. The function updates the spectrogram's
 * frequency range and calculates the necessary window sizes in both frequency and time
 * domains. It then identifies the peak in each window of the spectrogram and stores the
 * peak coordinates in the cmap structure.
 *
 * @param sgram Pointer to the spectrogram (`sgram_t`), which contains the frequency-time
 *              data and metadata, including frequency and time resolutions, as well as
 *              spectrogram dimensions.
 * @param delta_freq_Hz The width of the frequency window in Hertz. This defines the size
 *                       of the window used for frequency-domain peak detection.
 * @param delta_seg_sec The width of the time window in seconds. This defines the size
 *                       of the window used for time-domain peak detection.
 * @param max_freq_cutoff_Hz The maximum frequency cutoff in Hertz. The spectrogram will
 *                            be updated to reflect this new frequency range by adjusting
 *                            its frequency resolution and total number of frequency bins.
 *
 * @return A pointer to a `cmap_t` structure containing the coordinates of the peak values
 *         found within each frequency-time window of the spectrogram. The `cmap` structure
 *         includes metadata such as time and frequency step sizes, and maximum time and
 *         frequency values.
 */
cmap_t* make_cmap(sgram_t* sgram, double delta_freq_Hz, double delta_seg_sec, double max_freq_cutoff_Hz);


