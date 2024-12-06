#pragma once

typedef struct audio_t {
	double sample_rate;	// samples/sec
	size_t frames;
	size_t n_channels;
	double duration_sec;
	float* samples;
} audio_t;

/**
 * @brief Apply a bandwidth limit to a mono audio signal.
 *
 * This function limits the bandwidth of an audio signal by zeroing out frequency components
 * that are above a specified maximum frequency (`max_freq_Hz`). It first performs a Real-to-Complex
 * Fourier Transform (FFT) to convert the audio samples into the frequency domain, then truncates the
 * frequency components that exceed the `max_freq_Hz` threshold. Finally, it applies an inverse FFT to
 * transform the modified frequency domain data back into the time domain.
 *
 * The function ensures that the audio is mono, and the maximum frequency (`max_freq_Hz`) is constrained
 * to be below the Nyquist frequency, which is half the sample rate.
 *
 * @param audio Pointer to an `audio_t` structure containing the audio data.
 *        The audio must be mono (one channel), and the `samples` array should contain the audio samples.
 * @param max_freq_Hz The maximum frequency (in Hz) that will be allowed to pass through the signal.
 *        Frequencies above this value will be zeroed out.
 *        The `max_freq_Hz` should be less than the Nyquist frequency (half the sample rate).
 *
 * @note This function uses FFTW for performing Fourier transforms. The audio data is assumed to be in
 *       floating point format (e.g., `float` samples).
 *
 * @note The bandwidth limiting operation is performed by zeroing out frequency components beyond
 *       the specified `max_freq_Hz`. If the specified `max_freq_Hz` exceeds or equals the Nyquist frequency,
 *       the function will return without modifying the audio.
 *
 * @note The function does not perform any checks to ensure that the `audio` parameter is valid.
 *       It is the caller's responsibility to ensure the input is a valid `audio_t` object with appropriate
 *       sample data.
 */
void bwlimit_audio(audio_t* audio, double max_freq_Hz);

