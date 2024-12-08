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
 * @brief Allocates and initializes a stream_prop_t structure.
 *
 * This function allocates memory for a `stream_prop_t` structure and all
 * its internal fields, including arrays for storing `cpairs_t` pointers,
 * `hgram_t` pointers, and track names (`char*`). The number of tracks (`n_tracks`)
 * determines the size of the internal arrays.
 *
 * @param n_tracks The number of tracks to be allocated. This will determine the
 *                 size of the arrays inside the `stream_prop_t` structure.
 *
 * @return A pointer to the allocated and initialized `stream_prop_t` structure.
 *         On failure, it will return `NULL`, but this function asserts the
 *         validity of the allocated memory.
 *
 * @note This function uses `muse_assert_valid_ptr` to ensure that the memory was
 *       successfully allocated.
 */
stream_prop_t* init_stream_prop(size_t n_tracks);

/**
 * @brief Frees the memory allocated for a stream_prop_t structure.
 *
 * This function deallocates the memory used by the `stream_prop_t` structure
 * and all its internal fields. It frees the arrays for track names, `cpairs_t`
 * structures, and `hgram_t` structures. If any of these arrays contain pointers
 * to dynamically allocated memory, they are freed as well.
 *
 * @param stream_prop A pointer to the `stream_prop_t` structure to be freed.
 *                    The structure and all its internal arrays will be deallocated.
 *                    If `stream_prop` is `NULL`, the function does nothing.
 *
 * @note This function assumes that the `track_names`, `cpairs_tracks`, and `hgrams`
 *       arrays have been dynamically allocated and that `cpairs_t` structures
 *       may require their own cleanup (via `free_cpairs`).
 */
void free_stream_prop(stream_prop_t* stream_prop);

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
 * @param dir_src Directory of archived binary song files. Ensure string ends of "/".
 *
 * @throws PaError If there is an error initializing or starting the PortAudio stream, an assertion will be triggered.
 * @throws MemoryAllocationError If memory allocation for track samples or other data structures fails, an assertion will be triggered.
 */
void begin_live_recognition(double sample_rate, double frames_per_buffer_sec, char* dir_src);

