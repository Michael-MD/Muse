
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
#include "stream/live_recognition.h"

#define ms * 1e-3


int main() {

    double sample_rate = 48000;
    unsigned long frames_per_buffer_sec = 2;

    begin_live_recognition(sample_rate, frames_per_buffer_sec);


    return 0;

}
