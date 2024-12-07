#pragma once

// forward declaration
typedef struct cpairs_t cpairs_t;

typedef struct audio_t {
	double sample_rate;	// samples/sec
	size_t frames;
	size_t n_channels;
	double duration_sec;
	float* samples;
} audio_t;

/**
 * @brief Processes an audio signal by generating its spectrogram, constructing a constellation map,
 *        and extracting peak coordinates to form complex pairs.
 *
 * This function performs the following steps:
 * 1. Converts the input audio signal into a spectrogram using `make_sgram()`, which divides the signal
 *    into time-frequency segments.
 * 2. Constructs a constellation map from the spectrogram using `make_cmap()`, identifying peaks in
 *    the frequency-time domain.
 * 3. Extracts complex pairs from the constellation map using `make_cpairs()`, which are used for
 *    further analysis or signal processing.
 * 4. Frees the memory allocated for the spectrogram and constellation map before returning cpairs.
 *
 * @param audio A pointer to an `audio_t` structure containing the audio signal to be processed.
 *              The audio signal is assumed to be mono and should already be loaded in memory.
 *
 * @return A pointer to a `cpairs_t` structure containing the cpairs extracted from the
 *         constellation map, representing significant frequency-time peaks in the audio signal.
 *
 * @note The function uses hardcoded values for the parameters of the spectrogram, constellation map,
 *       and complex pairs extraction, which may need to be adjusted depending on the specific application.
 *       Temporary data dumps to files (commented out) are included for debugging purposes.
 */
cpairs_t* process_audio(audio_t* audio);