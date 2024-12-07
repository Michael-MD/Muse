#include "binary/archiving.h"
#include "binary/binary_io.h"
#include "audio/audio.h"
#include "audio/audio_io.h"
#include "fft/hashing.h"

void archive_wav_track(char* filename_src, char* filename_dest) {

	// --------------- import and process reference track ---------------
	audio_t audio_track;

	import_audio_track_mono(filename_src, &audio_track);
	cpairs_t* cpairs_track = process_audio(&audio_track);

	write_cpairs_to_binary(filename_dest, cpairs_track);

	free_cpairs(cpairs_track);
}
