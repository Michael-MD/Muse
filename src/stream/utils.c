#include "stream/utils.h"
#include <stdio.h>
#include <portaudio.h>
#include <assert.h>

PaStream* init_stream(int n_channels, double sample_rate, unsigned long frames_per_buffer,
    PaStreamCallback* stream_callback, void* user_data) {
    PaError paerr;

    paerr = Pa_Initialize();
    if (paerr != paNoError) {
        printf("port audio could not be initialized");
        assert(paerr != paNoError);
    }

    PaStream* stream;
    paerr = Pa_OpenDefaultStream(
        &stream,
        n_channels,
        0,
        paFloat32,
        sample_rate,
        frames_per_buffer,
        stream_callback,
        user_data
    );

    if (paerr != paNoError) {
        printf("PortAudio stream error: %s\n", Pa_GetErrorText(paerr));
        Pa_Terminate();
        assert(paerr != paNoError);
    }

    return stream;
}

void start_stream(PaStream* stream) {

    PaError paerr;

    // Start the audio stream
    paerr = Pa_StartStream(stream);
    if (paerr != paNoError) {
        printf("PortAudio stream start error: %s\n", Pa_GetErrorText(paerr));
        Pa_Terminate();
        assert(paerr != paNoError);
    }

    while (Pa_IsStreamActive(stream)) {
        Pa_Sleep(100);  // Sleep for a short while and then check if the stream is active
    }

    // Close the stream
    paerr = Pa_CloseStream(stream);
    if (paerr != paNoError) {
        printf("PortAudio stream close error: %s\n", Pa_GetErrorText(paerr));
        assert(paerr != paNoError);
    }

    // Terminate PortAudio
    Pa_Terminate();
}