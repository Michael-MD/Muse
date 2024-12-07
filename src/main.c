
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
#include "stream/utils.h"

#define ms * 1e-3

#define MAX_BAR_LENGTH 500  // Maximum length for the bar (number of # symbols)


static int stream_callback(const void* input_buffer, void* output_buffer,
    unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info,
    PaStreamCallbackFlags status_flags, void* user_data) {

    // Cast input_buffer to the correct type (assuming it's float for stereo or mono)
    float* in = (float*)input_buffer;

    // Calculate the average magnitude of the audio buffer (simplified volume calculation)
    float total_amplitude = 0.0f;
    for (unsigned long i = 0; i < frames_per_buffer; i++) {
        total_amplitude += in[i];  // Sum the absolute values of the samples
    }

    // Calculate the average amplitude
    float avg_amplitude = total_amplitude / frames_per_buffer;

    // Map the average amplitude to a volume bar length (scale the value)
    int bar_length = (int)(avg_amplitude * MAX_BAR_LENGTH) * 50;
    if (bar_length > MAX_BAR_LENGTH) {
        bar_length = MAX_BAR_LENGTH;  // Limit the bar length to the maximum value
    }

    // Print the volume bar as a sequence of `#` characters
    for (int i = 0; i < bar_length; i++) {
        printf("#");
    }
    printf("\n");

    return 0;
}



int main() {



    int n_channels = 1;
    double sample_rate = 44100;
    unsigned long frames_per_buffer = 5000;

    PaStreamCallback* scb = stream_callback;

    void* user_data = (void*)0;

    PaStream* stream = init_stream(n_channels, sample_rate, frames_per_buffer,
        scb, user_data);

    start_stream(stream);



    //// --------------- import and process sample clip ---------------
    //char* filename_clip = "C:/Users/61481/Documents/code/sample music/music/Bridgit Mendler - Hurricane sample noisey.wav";
    //audio_t audio_clip;
    //
    //import_audio_track_mono(filename_clip, &audio_clip);
    //cpairs_t* cpairs_clip = process_audio(&audio_clip);

    //// --------------- import and process reference track ---------------
    //char* filename_track = "C:/Users/61481/Documents/code/sample music/music/Bridgit Mendler - Hurricane sample long.wav";
    //audio_t audio_track;

    //import_audio_track_mono(filename_track, &audio_track);
    //cpairs_t* cpairs_track = process_audio(&audio_track);


    //// --------------- make hgram ---------------
    //float bin_width, bin_min, bin_max;
    //bin_width = 50; 
    //bin_min = 0;
    //bin_max = 15000;  // ms
    //hgram_t* hgram = init_hgram(bin_width, bin_min, bin_max);

    //// --------------- perform comparison ---------------
    //double freq_tol_Hz = 10, time_tol_ms = 10; // ms
    //update_hgram(hgram, cpairs_track, cpairs_clip, freq_tol_Hz, time_tol_ms);

    //dump_1d_arrayu32(hgram->bins, hgram->n_bins, "C:/Users/61481/Documents/code/sample music/dump/hgram/hgram.txt");

    //free_cpairs(cpairs_clip);
    //free_cpairs(cpairs_track);

    //return 0;

}
