#pragma once

// forward declaration
typedef struct audio_t audio_t;

typedef struct sgram_t {
	float** freqs;
	size_t n_freq;		// num. bins
	size_t n_seg;		// num. bins
	double max_freq_Hz;
	double max_seg_sec;
	double delta_seg_sec;
	double delta_seg_samples;
	double delta_freq_Hz;
} sgram_t;

/**
 * @brief Initializes and allocates memory for a spectrogram structure.
 *
 * This function allocates memory for a `sgram_t` structure, as well as memory for
 * a 2D array of floats representing a spectrogram with `n_freq` frequency bins and
 * `n_seg` time segments. The function returns a pointer to the initialized spectrogram.
 * First dimension is time segment, second is frequency.
 *
 * @param n_freq The number of frequency bins in the spectrogram.
 * @param n_seg The number of time segments in the spectrogram.
 *
 * @return A pointer to a `sgram_t` structure containing the allocated spectrogram data.
 *         Returns `NULL` if memory allocation fails.
 *
 * @note The caller is responsible for freeing the allocated memory when no longer needed.
 */
sgram_t* init_sgram(size_t n_freq, size_t n_seg);

/**
 * Frees the memory allocated for a `sgram_t` structure, including its 2D array
 * (`freqs`) for frequency values. This function ensures that all dynamically
 * allocated memory is properly released to avoid memory leaks.
 *
 * @param sgram Pointer to the `sgram_t` structure to be freed. This structure contains
 *              the spectrogram with allocated memory for frequency values.
 */
void free_sgram(sgram_t* sgram);

/**
 * @brief Creates a spectrogram for a mono audio signal.
 *
 * This function divides the audio signal into overlapping segments and computes
 * the Discrete Fourier Transform (DFT) of each segment to generate a spectrogram.
 * The spectrogram consists of frequency bins (from 0 to the Nyquist frequency)
 * for each time segment, where the amplitude of each bin represents the signal's
 * frequency content at that time.
 *
 * The function uses a sliding window approach with a specified window length
 * (`window_sec`) and hop size (`hop_sec`) to compute the spectrogram. The result
 * is stored in an `sgram_t` struct containing the spectrogram data and metadata.
 *
 * @param audio Pointer to the `audio_t` struct containing the audio signal.
 *              The audio signal must be mono (i.e., `n_channels == 1`).
 * @param window_sec Duration of each analysis window in seconds (e.g., 0.025 for 25 ms).
 * @param hop_sec Duration of the hop between successive windows in seconds.
 *
 * @return Pointer to an `sgram_t` struct containing the generated spectrogram.
 *         This struct includes the spectrogram's frequency bins, time segments,
 *         and metadata about the spectrogram's resolution.
 *
 * @note The input audio signal must be mono. The function assumes the sample rate
 *       of the audio signal is stored in the `audio->sample_rate` field.
 *       The function also ensures that the hop length is an even number to avoid
 *       potential issues during the DFT computation.
 */
sgram_t* make_sgram(audio_t* audio, double window_sec, double hop_sec);
