#include "binary/archiving.h"
#include "binary/binary_io.h"
#include "audio/audio.h"
#include "audio/audio_io.h"
#include "fft/hashing.h"
#include "audio/audio.h"
#include <dirent.h>
#include <stdio.h>

#define MAX_PATH_LENGTH 1024

void archive_wav_track(char* filename_src, char* filename_dst) {

	// --------------- import and process reference track ---------------
	audio_t audio_track;

	import_audio_track_mono(filename_src, &audio_track);
	cpairs_t* cpairs_track = process_audio(&audio_track);

	write_cpairs_to_binary(filename_dst, cpairs_track);

	free_cpairs(cpairs_track);
}


int archive_wav_dir(char* dir_src, char* dir_dst) {

    DIR* dir = opendir(dir_src);
    if (dir == NULL) {
        perror("Failed to open directory");
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {

        // ensure entry is regular file
        if (entry->d_type != DT_REG)
            continue;

        // ensure entry is wav file
        if (strcmp(get_extension(entry->d_name), "wav") != 0 && strcmp(get_extension(entry->d_name), "WAV") != 0)
            continue;
        
        printf("archiving %s...\n", entry->d_name);

        char filename_src[MAX_PATH_LENGTH], filename_dst[MAX_PATH_LENGTH];
        
        // copy directory into filename
        strcpy(filename_src, dir_src);
        strcpy(filename_dst, dir_dst);

        // append file name onto directory
        strcat(filename_src, entry->d_name);
        // change filename extension from .wav to .bin
        change_extension(entry->d_name, "bin");
        strcat(filename_dst, entry->d_name);

        archive_wav_track(filename_src, filename_dst);
    }

    if (closedir(dir) == -1) {
        perror("Failed to close directory");
        return 1;
    }

    return 0;
}
