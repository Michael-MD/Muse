#pragma once
#include "fft/dft.h"
#include <stdlib.h>
#include <assert.h>

void dft_r2c(float* samples, size_t N, fftwf_complex* complex_freqs) {
    fftwf_plan plan;

    // Create the FFT plan
    plan = fftwf_plan_dft_r2c_1d(N, samples, complex_freqs, FFTW_ESTIMATE);
    assert(plan != NULL);

    // Execute the FFT
    fftwf_execute(plan);

    // Free the FFT plan
    fftwf_destroy_plan(plan);
}

void dft_c2r(float* samples, size_t N, fftwf_complex* complex_freqs) {
    fftwf_plan plan;

    // Create the FFT plan
    plan = fftwf_plan_dft_c2r_1d(N, complex_freqs, samples, FFTW_ESTIMATE);
    assert(plan != NULL);

    // Execute the FFT
    fftwf_execute(plan);

    // Free the FFT plan
    fftwf_destroy_plan(plan);
}

void dft_r2abs2(float* samples, size_t N, float* freq_abs) {
    
    // Allocate memory for output
    fftwf_complex* complex_freqs = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (N / 2 + 1));
    
    dft_r2c(samples, N, complex_freqs);

    for (size_t n = 0; n < N / 2 + 1; n++) {
        freq_abs[n] = (complex_freqs[n][0] * complex_freqs[n][0]) + (complex_freqs[n][1] * complex_freqs[n][1]);
    }

    fftwf_free(complex_freqs);
}