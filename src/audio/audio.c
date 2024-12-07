#include "audio/audio.h"
#include "fft/dft.h"
#include <assert.h>
#include <fftw3.h>
#include "fft/spectrogram.h"
#include "fft/constellation.h"
#include "fft/hashing.h"

#include "debugging/dumping.h"

#define ms * 1e-3

cpairs_t* process_audio(audio_t* audio) {

    sgram_t* sgram = make_sgram(audio, 50 ms, 10 ms);

    //dump_2d_arrayf32(sgram->freqs, sgram->n_seg, sgram->n_freq, "C:/Users/61481/Documents/code/sample music/dump/sgram/sgram sample.txt");
    //printf("%li %li", sgram->n_seg, sgram->n_freq);

    cmap_t* cmap = make_cmap(sgram, 100, 250 ms, 10000);

    //dump_2d_arrayu32(cmap->freqs, cmap->n_stacks_seg, cmap->n_stacks_freq, "C:/Users/61481/Documents/code/sample music/dump/cmap/cmap freq sample.txt");
    //dump_2d_arrayu32(cmap->times, cmap->n_stacks_seg, cmap->n_stacks_freq, "C:/Users/61481/Documents/code/sample music/dump/cmap/cmap times sample.txt");
    //printf("%li %li", cmap->n_stacks_seg, cmap->n_stacks_freq);

    double tzone_delta_freq_Hz = 500, tzone_delta_time_sec = 1000 ms, hop_freq_Hz = 500, hop_time_sec = 200 ms, f = 1;
    cpairs_t* cpairs = make_cpairs(cmap, tzone_delta_freq_Hz, tzone_delta_time_sec, hop_freq_Hz, hop_time_sec, f);

    //dump_cpairs_to_file(cpairs, "C:/Users/61481/Documents/code/sample music/dump/cpairs/cpairs sample.txt");

    free_sgram(sgram);
    free_cmap(cmap);

    return cpairs;
}