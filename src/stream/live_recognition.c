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
#include <stdlib.h>

static int stream_callback(const void* input_buffer, void* output_buffer,
	unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info,
	PaStreamCallbackFlags status_flags, void* user_data) {

	// stream timing information
	static float time_since_match_check_sec = 0;
	static float match_check_period_sec = 1;
	static float timeout_sec = 20;
	static double elapsed_time_sec = 0;

	stream_prop_t* stream_prop = (stream_prop_t*)user_data;
	
	// arrange incoming signal in convenient data structure
	stream_prop->audio_clip->samples = (float*)input_buffer;	// reinterpret input stream as float stream

	//dump_1d_arrayf32(stream_prop->audio_clip->samples, frames_per_buffer, "C:/Users/61481/Documents/code/sample music/dump/audio/captured.txt");

	// perform processing on recieved data
	cpairs_t* cpairs_clip = process_audio(stream_prop->audio_clip);

	// update histograms
	double freq_tol_Hz = 10, time_tol_ms = 10; // ms

	for (size_t track = 0; track < stream_prop->n_tracks; track++) {
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
		for (size_t track = 0; track < stream_prop->n_tracks; track++) {
			bool match_found = score_hgram(stream_prop->hgrams[track], 100);

			// end stream if match has been found
			if (match_found) {
				printf("Match found: %s.\n", stream_prop->track_names[track]);

				return paAbort;
			}
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


void begin_live_recognition(double sample_rate, double frames_per_buffer_sec) {

	unsigned int frames_per_buffer = frames_per_buffer_sec * sample_rate;

	// setup clip audio data structure
	audio_t audio_clip = (audio_t){
		.sample_rate = sample_rate,
		.frames = frames_per_buffer,
		.n_channels = 1,
		.duration_sec = frames_per_buffer * frames_per_buffer,
		//.samples  // this will be set to the incoming data in the callback
	};

	size_t n_tracks = 2;

	// create data structure to be passed to callback function
	stream_prop_t stream_prop = (stream_prop_t){
		.audio_clip = &audio_clip,
		.cpairs_tracks = (cpairs_t**)malloc(sizeof(cpairs_t*) * n_tracks),
		.hgrams = (hgram_t**)malloc(sizeof(hgram_t*) * n_tracks),
		.track_names = (char**)malloc(sizeof(char*) * n_tracks),
		.n_tracks = n_tracks,
	};

	// add tracks
	// *****TEMPORARY*****: imports since track from binary file
	char* filename_track_1 = "C:/Users/61481/Documents/code/sample music/database/Bridgit Mendler - Hurricane.bin";
	cpairs_t* cpairs_track_1 = read_cpairs_from_binary(filename_track_1);

	char* filename_track_2 = "C:/Users/61481/Documents/code/sample music/database/Jessica Darrow - Surface Pressure.bin";
	cpairs_t* cpairs_track_2 = read_cpairs_from_binary(filename_track_2);

	stream_prop.cpairs_tracks[0] = cpairs_track_1;
	stream_prop.cpairs_tracks[1] = cpairs_track_2;

	stream_prop.track_names[0] = "Bridgit Mendler - Hurricane";
	stream_prop.track_names[1] = "Jessica Darrow - Surface Pressure";

	// make histogram for each track
	for (size_t track = 0; track < stream_prop.n_tracks; track++) {

		float bin_width, bin_min, bin_max;
		bin_width = 50;
		bin_min = 0;
		bin_max = stream_prop.cpairs_tracks[track]->duration_sec * 1e3;
		stream_prop.hgrams[track] = init_hgram(bin_width, bin_min, bin_max);

	}

	// start listening
	printf("stream started.\n");

	int n_channels = 1;
    PaStream* stream = init_stream(n_channels, sample_rate, frames_per_buffer, stream_callback, (void*)&stream_prop);
    start_stream(stream);

	free_cpairs(cpairs_track_1);
	free_cpairs(cpairs_track_2);
	free(stream_prop.cpairs_tracks);
	free_hgram(stream_prop.hgrams);
	free(stream_prop.track_names);
}