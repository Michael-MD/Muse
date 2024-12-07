
#include "fft/dft.h"
#include "debugging/asserts.h"
#include <stdlib.h>
#include "fft/spectrogram.h"
#include "fft/constellation.h"
#include "audio/audio.h"
#include "audio/audio_io.h"
#include "debugging/dumping.h"
#include <stdio.h>
#include "fft/hashing.h"

#define ms * 1e-3

int main() {
    char* filename = "C:/Users/61481/Documents/code/sample music/music/Bridgit Mendler - Hurricane sample.wav";
    audio_t audio;
    
    import_audio_track_mono(filename, &audio);
    sgram_t* sgram = make_sgram(&audio, 50 ms, 10 ms);

    //dump_2d_arrayf32(sgram->freqs, sgram->n_seg, sgram->n_freq, "C:/Users/61481/Documents/code/sample music/dump/sgram/sgram sample.txt");
    //printf("%li %li", sgram->n_seg, sgram->n_freq);

    cmap_t* cmap = make_cmap(sgram, 50, 250 ms, 15000);

    //dump_2d_arrayu32(cmap->freqs, cmap->n_stacks_seg, cmap->n_stacks_freq, "C:/Users/61481/Documents/code/sample music/dump/cmap/cmap freq sample.txt");
    //dump_2d_arrayu32(cmap->times, cmap->n_stacks_seg, cmap->n_stacks_freq, "C:/Users/61481/Documents/code/sample music/dump/cmap/cmap times sample.txt");
    //printf("%li %li", cmap->n_stacks_seg, cmap->n_stacks_freq);

    double tzone_delta_freq_Hz = 200, tzone_delta_time_sec = 1000 ms, hop_freq_Hz = 50, hop_time_sec = 250 ms, f = 1;
    cpairs_t* cpairs = make_cpairs(cmap, tzone_delta_freq_Hz, tzone_delta_time_sec, hop_freq_Hz, hop_time_sec, f);

    dump_cpairs_to_file(cpairs, "C:/Users/61481/Documents/code/sample music/dump/cpairs/cpairs sample.txt");

    return 0;

    // TODO: make deallocation functions for sgram, cmap, cpairs
}
