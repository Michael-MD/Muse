#include "stream/live_recognition.h"
#include "stream/utils.h"
#include "fft/hashing.h"
#include "scoring/histogram/histogram.h"
#include "audio/audio_io.h"
#include "scoring/matching.h"
#include "binary/binary_io.h"
#include <stdbool.h>
#include <stdio.h>
#include "debugging/dumping.h"
#include "debugging/asserts.h"
#include <stdlib.h>
#include <omp.h>

stream_prop_t* init_stream_prop(size_t n_tracks) {
	
	stream_prop_t* stream_prop = (stream_prop_t*)malloc(sizeof(stream_prop_t));
	muse_assert_valid_ptr(stream_prop, "invalid stream_prop_t* memory allocation.");

	// determine number of tracks to be loaded
	stream_prop->n_tracks = n_tracks;

	// allocate memory for track data
	stream_prop->cpairs_tracks = (cpairs_t**)malloc(sizeof(cpairs_t*) * stream_prop->n_tracks);
	muse_assert_valid_ptr(stream_prop->cpairs_tracks, "invalid cpairs_t** memory allocation.");

	stream_prop->hgrams = (hgram_t**)malloc(sizeof(hgram_t*) * stream_prop->n_tracks);
	muse_assert_valid_ptr(stream_prop->hgrams, "invalid hgram_t** memory allocation.");

	stream_prop->track_names = (char**)malloc(sizeof(char*) * stream_prop->n_tracks);
	muse_assert_valid_ptr(stream_prop->track_names, "invalid char** memory allocation.");

	return stream_prop;
}

void free_stream_prop(stream_prop_t* stream_prop) {
	if (stream_prop == NULL) {
		return; // If the pointer is NULL, there's nothing to free
	}

	// Free track names (each element is a string, so we need to free each string)
	for (size_t i = 0; i < stream_prop->n_tracks; i++) {
		free(stream_prop->track_names[i]);  // Free each string
	}

	// Free the track names array itself
	free(stream_prop->track_names);

	// Free the cpairs_tracks array (assuming each cpairs_t* in the array may have been allocated)
	for (size_t i = 0; i < stream_prop->n_tracks; i++) {
		free_cpairs(stream_prop->cpairs_tracks[i]);  // Free each cpairs_t (if needed)
	}

	// Free the cpairs_tracks array itself
	free(stream_prop->cpairs_tracks);

	// Free the hgrams array (assuming each hgram_t* in the array may have been allocated)
	for (size_t i = 0; i < stream_prop->n_tracks; i++) {
		free(stream_prop->hgrams[i]);  // Free each hgram_t (if needed)
	}

	// Free the hgrams array itself
	free(stream_prop->hgrams);

	// Finally, free the stream_prop structure itself
	free(stream_prop);
}

static int stream_callback(const void* input_buffer, void* output_buffer,
	unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info,
	PaStreamCallbackFlags status_flags, void* user_data) {

	// stream timing information
	static float time_since_match_check_sec = 0;
	static float match_check_period_sec = 1;
	static float timeout_sec = 20;
	static double elapsed_time_sec = 0;
	static double min_run_duration_sec = 2;		// minimum time stream must run before it can be aborted

	stream_prop_t* stream_prop = (stream_prop_t*)user_data;
	
	// arrange incoming signal in convenient data structure
	stream_prop->audio_clip->samples = (float*)input_buffer;	// reinterpret input stream as float stream

	//dump_1d_arrayf32(stream_prop->audio_clip->samples, frames_per_buffer, "C:/Users/61481/Documents/code/sample music/dump/audio/captured.txt");

	// perform processing on recieved data
	cpairs_t* cpairs_clip = process_audio(stream_prop->audio_clip);

	// update histograms
	double freq_tol_Hz = 10, time_tol_ms = 10; // ms

	int track;
	#pragma omp parallel for private(track)
	for (track = 0; track < stream_prop->n_tracks; track++) {
		update_hgram(stream_prop->hgrams[track], stream_prop->cpairs_tracks[track], cpairs_clip, freq_tol_Hz, time_tol_ms, elapsed_time_sec * 1e3);
	}
	
	// free clip's cpairs
	free_cpairs(cpairs_clip);

	// the remaining code requires the elapsed time from the end of the recieved segment
	elapsed_time_sec += frames_per_buffer / stream_prop->audio_clip->sample_rate;

	// check periodically for statistically significant match
	if (elapsed_time_sec > time_since_match_check_sec + match_check_period_sec) {

		// update time since check
		time_since_match_check_sec = elapsed_time_sec;

		// check for match in each histogram
		int track;
		bool match_found = false;
		char* match_name;
		#pragma omp parallel for private(track)
		for (track = 0; track < stream_prop->n_tracks; track++) {
			bool hgram_match = score_hgram(stream_prop->hgrams[track], 100);

			// end stream if match has been found
			if (hgram_match) {
				match_name = stream_prop->track_names[track];
				match_found = true;
			}
		}

		if (match_found && elapsed_time_sec > min_run_duration_sec) {
			printf("Match found: %s.\n", match_name);
			return paAbort;
		}

	}

	//dump_1d_arrayu32(stream_prop->hgram->bins, stream_prop->hgram->n_bins, "C:/Users/61481/Documents/code/sample music/dump/hgram/hgram.txt");

	// after certain period time out
	if (elapsed_time_sec > timeout_sec) {
		printf("No match found.\n");
		return paAbort;
	}

	return paContinue;
}


void begin_live_recognition(double sample_rate, double frames_per_buffer_sec, char* dir_src) {

	unsigned int frames_per_buffer = frames_per_buffer_sec * sample_rate;

	// setup clip audio data structure
	audio_t audio_clip = (audio_t){
		.sample_rate = sample_rate,
		.frames = frames_per_buffer,
		.n_channels = 1,
		.duration_sec = frames_per_buffer * frames_per_buffer,
		//.samples  // this will be set to the incoming data in the callback
	};


	stream_prop_t* stream_prop = read_tracks_from_binary_dir(dir_src);

	stream_prop->audio_clip = &audio_clip;

	// start listening
	printf("stream started.\n");

	int n_channels = 1;
    PaStream* stream = init_stream(n_channels, sample_rate, frames_per_buffer, stream_callback, (void*)stream_prop);
    start_stream(stream);

	
	free_stream_prop(stream_prop);
}