#include "audio/audio_io.h"
#include "debugging/asserts.h"
#include "audio/audio.h"
#include <stdlib.h>

void read_wav_file(char* filename, audio_t* audio) {
	
	SF_INFO sf_info;
	SNDFILE* file = sf_open(filename, SFM_READ, &sf_info);
	muse_assert_valid_ptr(file, ("error opening file '%s': %s\n", filename, sf_strerror(NULL)));

	// populate audio metadata
	audio->sample_rate = sf_info.samplerate;
	audio->frames = sf_info.frames;
	audio->n_channels = sf_info.channels;
	audio->duration_sec = (double)sf_info.frames / sf_info.samplerate;

	// allocate samples memory
	audio->samples = (float*)malloc(sf_info.frames * sf_info.channels * sizeof(float));
	muse_assert_valid_ptr(audio->samples, "could not allocate memory for audio samples.");

	// read sound data into audio_.samples
	sf_readf_float(file, audio->samples, sf_info.frames);

	sf_close(file);

}


void make_audio_mono(audio_t* audio) {

	if (audio->n_channels > 1) {
		float* buffer = (float*)malloc(audio->frames * sizeof(float));
		muse_assert_valid_ptr(buffer, "could not allocate buffer for stereo to mono conversion.");

		// set buffer to zero since channels will be added up
		// per frame
		memset(buffer, 0, audio->frames * sizeof(float));

		for (size_t frame = 0; frame < audio->frames; frame++) {

			// calc average of channels
			for (size_t channel = 0; channel < audio->n_channels; channel++) {
				buffer[frame] += audio->samples[frame * audio->n_channels + channel];
			}
			buffer[frame] /= audio->n_channels;

		}

		// free previously sterio-allocated memory
		free(audio->samples);

		// assign mono audio to audio samples
		audio->samples = buffer;
	}

	// update audio meta information
	audio->n_channels = 1;
}

void import_audio_track_mono(char* filename, audio_t* audio) {

	read_wav_file(filename, audio);
	make_audio_mono(audio);

}