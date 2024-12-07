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


static const char* get_extension(const char* filename) {
    // Find the last period in the filename
    const char* dot = strrchr(filename, '.');

    // If there is no period or if it's at the beginning (i.e., hidden file), return NULL
    if (!dot || dot == filename) {
        return NULL;
    }

    // Return the substring after the last period
    return dot + 1;  // Skip the period itself
}

static int change_extension(char* filename, const char* new_extension) {
    // Find the last dot in the filename (if any)
    char* dot = strrchr(filename, '.');

    if (dot == NULL) {
        // No dot found, so append the new extension
        strcat(filename, ".");
        strcat(filename, new_extension);
    }
    else {
        // Replace the extension (after the last dot)
        strcpy(dot + 1, new_extension);
    }

    return 0; // Return success
}

void archive_wav_dir(char* dir_src, char* dir_dst) {

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
        if (strcmp(get_extension(entry->d_name), "wav") != 0)
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
}
