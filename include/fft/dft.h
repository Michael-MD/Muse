#pragma once
#include <fftw3.h>

// forward declaration
typedef struct track_t track_t;

void dft_r2c(float* samples, size_t N, fftwf_complex* complex_freqs);
void dft_c2r(float* samples, size_t N, fftwf_complex* complex_freqs);
void dft_r2abs2(float* samples, size_t N, float* freq_abs);
