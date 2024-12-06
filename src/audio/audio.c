#include "audio/audio.h"
#include "fft/dft.h"
#include <assert.h>
#include <fftw3.h>

void bwlimit_audio(audio_t* audio, double max_freq_Hz) {

	// ensure audio is mono
	assert(audio->n_channels == 1);

	size_t N = audio->frames;
	fftwf_complex* complex_freqs = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (N / 2 + 1));
	dft_r2c(audio->samples, N, complex_freqs);

	// find index of maximal frequency
	double nyquist_Hz = audio->sample_rate / 2;

	// if the bandwidth limit is at or beyond nyquist then the request 
	// is nonsense
	if (max_freq_Hz >= nyquist_Hz)
		return;

	size_t max_freq_samples = (max_freq_Hz / nyquist_Hz) * N;

	// perform bandwidth limit
	for (size_t freq = max_freq_samples; freq < N; freq++) {
		complex_freqs[freq][0] = 0;
		complex_freqs[freq][1] = 0;
	}

	dft_c2r(audio->samples, N, complex_freqs);

	fftwf_free(complex_freqs);
}
