#pragma once

#include <portaudio.h>

//forward declaation
typedef struct audio_t audio_t;
typedef struct cpairs_t cpairs_t;
typedef struct hgram_t hgram_t;

typedef struct stream_prop_t {
	audio_t* audio_clip;
	cpairs_t** cpairs_tracks;
	hgram_t** hgrams;
	char** track_names;
	size_t n_tracks;
} stream_prop_t;


/**
 * Initializes and starts the live audio recognition process by setting up the audio clip,
 * importing a reference track, processing it, and starting the audio stream.
 *
 * This function performs several tasks to set up live audio recognition:
 * 1. It configures an `audio_clip` structure that will hold data for incoming audio during the recognition process.
 * 2. It loads and processes a reference track to create a set of "control pairs" (`cpairs_track`), which represent
 *    the features of the track used for comparison during live recognition.
 * 3. It initializes a histogram (`hgram`) that will be used to store the recognition data as it is processed.
 * 4. It creates a `stream_prop_t` structure that holds the audio clip, control pairs, and histogram data for use in the
 *    callback function during the audio stream.
 * 5. Finally, it initializes and starts the PortAudio stream to begin capturing live audio and processing it in real time.
 *
 * The function assumes a pre-recorded reference track is available for comparison during recognition.
 * It currently uses a hardcoded file path for the reference track, but this can be modified for more dynamic use cases.
 * The audio stream is set to operate with a mono channel, but this can be adjusted as needed.
 *
 * @param sample_rate The sample rate (in Hz) of the incoming audio stream.
 * @param frames_per_buffer_sec The duration per buffer in the audio stream, used to define the buffer size per frame.
 *
 * @throws PaError If there is an error initializing or starting the PortAudio stream, an assertion will be triggered.
 * @throws MemoryAllocationError If memory allocation for track samples or other data structures fails, an assertion will be triggered.
 */
void begin_live_recognition(double sample_rate, double frames_per_buffer_sec);