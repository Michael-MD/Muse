#include "stream/live_recognition.h"
#include "stream/utils.h"
#include "fft/hashing.h"
#include "scoring/histogram/histogram.h"
#include "audio/audio_io.h"
#include "scoring/matching.h"
#include <stdbool.h>
#include <stdio.h>
#include "debugging/dumping.h"

static int stream_callback(const void* input_buffer, void* output_buffer,
	unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info,
	PaStreamCallbackFlags status_flags, void* user_data) {

	// stream timing information
	static float time_since_match_check_sec = 0;
	static float match_check_period_sec = 1;
	static float timeout_sec = 20;
	static double elapsed_time_sec = 0;

	stream_prop_t* stream_prop = (stream_prop_t*)user_data;

	elapsed_time_sec += frames_per_buffer / stream_prop->audio_clip->sample_rate;
	
	// arrange incoming signal in convenient data structure
	stream_prop->audio_clip->samples = (float*)input_buffer;	// reinterpret input stream as float stream

	//dump_1d_arrayf32(stream_prop->audio_clip->samples, frames_per_buffer, "C:/Users/61481/Documents/code/sample music/dump/audio/captured.txt");

	// perform processing on recieved data
	cpairs_t* cpairs_clip = process_audio(stream_prop->audio_clip);

	// update histogram
	double freq_tol_Hz = 10, time_tol_ms = 10; // ms
	update_hgram(stream_prop->hgram, stream_prop->cpairs_track, cpairs_clip, freq_tol_Hz, time_tol_ms);
	
	// free clip's cpairs
	free_cpairs(cpairs_clip);

	// check periodically for statistically significant match
	if (elapsed_time_sec > time_since_match_check_sec + match_check_period_sec) {

		// update time since check
		time_since_match_check_sec = elapsed_time_sec;

		// check for match
		bool match_found = score_hgram(stream_prop->hgram, 100);

		// end stream if match has been found
		if (match_found) {
			printf("Match found.\n");

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

	// *****TEMPORARY*****: identifies single preloadewd track
	// import and process reference track
	char* filename_track = "C:/Users/61481/Documents/code/sample music/music/Bridgit Mendler - Hurricane sample long.wav";
	audio_t audio_track;
	
	import_audio_track_mono(filename_track, &audio_track);
	cpairs_t* cpairs_track = process_audio(&audio_track);

	// make empty histogram 
	float bin_width, bin_min, bin_max;
	bin_width = 50;
	bin_min = 0;
	bin_max = audio_track.duration_sec * 1e3;
	hgram_t* hgram = init_hgram(bin_width, bin_min, bin_max);

	// create data structure to be passed to callback function
	stream_prop_t stream_prop = (stream_prop_t){
		.audio_clip = &audio_clip,
		.cpairs_track = cpairs_track,
		.hgram = hgram,
	};

	// start listening
	printf("stream started.\n");

	int n_channels = 1;
    PaStream* stream = init_stream(n_channels, sample_rate, frames_per_buffer, stream_callback, (void*)&stream_prop);
    start_stream(stream);

	free_cpairs(cpairs_track);
	free_hgram(hgram);
}