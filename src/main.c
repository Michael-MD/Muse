
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
#include "scoring/histogram/histogram.h"
#include "scoring/matching.h"

#define ms * 1e-3

int main() {

    // --------------- import and process sample clip ---------------
    char* filename_clip = "C:/Users/61481/Documents/code/sample music/music/Bridgit Mendler - Hurricane sample.wav";
    audio_t audio_clip;
    
    import_audio_track_mono(filename_clip, &audio_clip);
    cpairs_t* cpairs_clip = process_audio(&audio_clip);

    // --------------- import and process reference track ---------------
    char* filename_track = "C:/Users/61481/Documents/code/sample music/music/Bridgit Mendler - Hurricane.wav";
    audio_t audio_track;

    import_audio_track_mono(filename_track, &audio_track);
    cpairs_t* cpairs_track = process_audio(&audio_track);


    // --------------- make hgram ---------------
    float bin_width, bin_min, bin_max;
    bin_width = 250;  // <- perhaps 20? ms
    bin_min = 0;
    bin_max = 10000;  // ms
    hgram_t* hgram = init_hgram(bin_width, bin_min, bin_max);

    // --------------- perform comparison ---------------
    double freq_tol_Hz = 250, time_tol_ms = 250; // ms
    update_hgram(hgram, cpairs_track, cpairs_clip, freq_tol_Hz, time_tol_ms);

    free_cpairs(cpairs_clip);
    free_cpairs(cpairs_track);

    return 0;

}
