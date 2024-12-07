#pragma once
#include <portaudio.h>
#include "audio/audio.h"

// forward declarations
typedef struct hgram_t hgram_t;
typedef struct cpairs_t cpairs_t;

/**
 * Initializes a PortAudio stream with the specified parameters.
 *
 * This function initializes PortAudio, creates a stream with the provided settings,
 * and returns a handle to the opened stream. The stream is set up to use floating-point
 * audio samples, with the specified number of channels, sample rate, and frames per buffer.
 * It also sets up a callback function to process audio data and a user data pointer.
 *
 * @param n_channels The number of audio channels (e.g., 1 for mono, 2 for stereo).
 * @param sample_rate The sample rate in Hz (e.g., 44100, 48000).
 * @param frames_per_buffer The number of frames per buffer that the callback will process.
 * @param stream_callback A callback function that will be called to process the audio stream.
 * @param user_data A pointer to user-specific data that will be passed to the callback function.
 *
 * @return A pointer to a `PaStream` object representing the initialized audio stream.
 *
 * @throws PaError If there is an error initializing PortAudio or opening the stream, an assertion is triggered.
 */
PaStream* init_stream(int n_channels, double sample_rate, unsigned long frames_per_buffer,
    PaStreamCallback* stream_callback, void* user_data);

/**
 * Starts a PortAudio stream and processes audio data until the stream is no longer active.
 *
 * This function starts the stream created by `init_stream`. It enters a loop that checks if
 * the stream is still active and sleeps for 100 milliseconds between checks. When the stream
 * is no longer active, it stops the stream and closes the PortAudio stream.
 * The function also terminates PortAudio after closing the stream.
 *
 * @param stream A pointer to a `PaStream` object that was returned by `init_stream`.
 *
 * @throws PaError If there is an error starting, stopping, or closing the stream, an assertion is triggered.
 */
void start_stream(PaStream* stream);