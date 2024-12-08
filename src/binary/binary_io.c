
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "fft/hashing.h"
#include "binary/binary_io.h"
#include <dirent.h>
#include "scoring/histogram/histogram.h"
#include "debugging/asserts.h"
#include "stream/live_recognition.h"

#define MAX_PATH_LENGTH 1024

// Comparison function for sorting by hash
static int compare_by_hash(const void* a, const void* b) {
    cpair_t* cpair_a = (cpair_t*)a;
    cpair_t* cpair_b = (cpair_t*)b;

    // First compare freq1_Hz
    if (cpair_a->hash.freq1_Hz < cpair_b->hash.freq1_Hz) {
        return -1;  // a comes before b
    }
    else if (cpair_a->hash.freq1_Hz > cpair_b->hash.freq1_Hz) {
        return 1;   // a comes after b
    }

    // If freq1_Hz is the same, compare freq2_Hz
    if (cpair_a->hash.freq2_Hz < cpair_b->hash.freq2_Hz) {
        return -1;
    }
    else if (cpair_a->hash.freq2_Hz > cpair_b->hash.freq2_Hz) {
        return 1;
    }

    // If both freq1_Hz and freq2_Hz are the same, compare delta_t_ms
    if (cpair_a->hash.delta_t_ms < cpair_b->hash.delta_t_ms) {
        return -1;
    }
    else if (cpair_a->hash.delta_t_ms > cpair_b->hash.delta_t_ms) {
        return 1;
    }

    // If all fields are equal
    return 0;
}

void write_cpairs_to_binary(const char* filename, const cpairs_t* cpairs) {
    FILE* file = fopen(filename, "wb");  // Open the file for binary writing
    if (!file) {
        perror("Failed to open file for writing");
        return -1;
    }

    // store cpairs sorted by hash for quicker access
    qsort(cpairs->cpairs, cpairs->n_cpairs, sizeof(cpair_t), compare_by_hash);

    // Write the metadata for cpairs_t (n_cpairs, max_freq_Hz, duration_sec)
    if (fwrite(&cpairs->n_cpairs, sizeof(size_t), 1, file) != 1 ||
        fwrite(&cpairs->max_freq_Hz, sizeof(double), 1, file) != 1 ||
        fwrite(&cpairs->duration_sec, sizeof(double), 1, file) != 1) {
        perror("Failed to write cpairs metadata to file");
        fclose(file);
        return -1;
    }

    // Write each cpair_t in cpairs
    for (size_t i = 0; i < cpairs->n_cpairs; i++) {
        // Write the hash data (freq1_Hz, freq2_Hz, delta_t_ms)
        if (fwrite(&cpairs->cpairs[i].hash, sizeof(hash_t), 1, file) != 1) {
            perror("Failed to write hash data to file");
            fclose(file);
            return -1;
        }

        // Write the offset_start_ms data
        if (fwrite(&cpairs->cpairs[i].offset_start_ms, sizeof(float), 1, file) != 1) {
            perror("Failed to write offset_start_ms to file");
            fclose(file);
            return -1;
        }
    }

    // Close the file after writing
    fclose(file);
}


cpairs_t* read_cpairs_from_binary(const char* filename) {

    FILE* file = fopen(filename, "rb");  // Open the file for binary reading
    if (!file) {
        perror("Failed to open file for reading");
        return -1;
    }

    cpairs_t* cpairs = (cpairs_t*)malloc(sizeof(cpairs_t));

    // Read the metadata for cpairs_t (n_cpairs, max_freq_Hz, duration_sec)
    if (fread(&cpairs->n_cpairs, sizeof(size_t), 1, file) != 1 ||
        fread(&cpairs->max_freq_Hz, sizeof(double), 1, file) != 1 ||
        fread(&cpairs->duration_sec, sizeof(double), 1, file) != 1) {
        perror("Failed to read cpairs metadata from file");
        fclose(file);
        free_cpairs(cpairs);
        return -1;
    }

    // Allocate memory for the cpairs array
    cpairs->cpairs = (cpair_t*)malloc(cpairs->n_cpairs * sizeof(cpair_t));
    if (cpairs->cpairs == NULL) {
        perror("Failed to allocate memory for cpairs array");
        fclose(file);
        free_cpairs(cpairs);
        return -1;
    }

    // Read each cpair_t from the file
    for (size_t i = 0; i < cpairs->n_cpairs; i++) {
        // Read the hash data (freq1_Hz, freq2_Hz, delta_t_ms)
        if (fread(&cpairs->cpairs[i].hash, sizeof(hash_t), 1, file) != 1) {
            perror("Failed to read hash data from file");
            free(cpairs->cpairs);  // Free the allocated memory
            fclose(file);
            free_cpairs(cpairs);
            return -1;
        }

        // Read the offset_start_ms data
        if (fread(&cpairs->cpairs[i].offset_start_ms, sizeof(float), 1, file) != 1) {
            perror("Failed to read offset_start_ms from file");
            free(cpairs->cpairs);  // Free the allocated memory
            fclose(file);
            free_cpairs(cpairs);
            return -1;
        }
    }

    // Close the file after reading
    fclose(file);

    return cpairs;
}

const char* get_extension(const char* filename) {
    // Find the last period in the filename
    const char* dot = strrchr(filename, '.');

    // If there is no period or if it's at the beginning (i.e., hidden file), return NULL
    if (!dot || dot == filename) {
        return NULL;
    }

    // Return the substring after the last period
    return dot + 1;  // Skip the period itself
}

int change_extension(char* filename, const char* new_extension) {
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

size_t count_tracks_dir(char* dir_src) {

    // --------- open directory for file counting --------- 
    DIR* dir = opendir(dir_src);
    if (dir == NULL) {
        perror("Failed to open directory");
        return 1;
    }


    // determine number of tracks to be loaded in
    size_t n_tracks = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {

        // ensure entry is regular file
        if (entry->d_type != DT_REG)
            continue;

        // ensure entry is wav file
        //if (strcmp(get_extension(entry->d_name), "wav") != 0)
            //continue;

        n_tracks++;
    }

    if (closedir(dir) == -1) {
        perror("Failed to close directory");
        return NULL;
    }

    return n_tracks;

}

stream_prop_t* read_tracks_from_binary_dir(char* dir_src) {

    stream_prop_t* stream_prop = init_stream_prop(count_tracks_dir(dir_src));

    // open directory to start loading in binary files
    DIR* dir = opendir(dir_src);
    if (dir == NULL) {
        perror("Failed to open directory");
        return 1;
    }

    // determine number of tracks to be loaded in
    struct dirent* entry;
    size_t track = 0;
    char filename[MAX_PATH_LENGTH];
    while ((entry = readdir(dir)) != NULL) {

        // ensure entry is regular file
        if (entry->d_type != DT_REG)
            continue;

        // ensure entry is bin file
        if (strcmp(get_extension(entry->d_name), "bin") != 0)
            continue;

        // same file name as track name, this will include the extension
        stream_prop->track_names[track] = (char*)malloc(sizeof(entry->d_name));
        muse_assert_valid_ptr(stream_prop->track_names[track], "invalid track name memory allocation");

        strcpy(stream_prop->track_names[track], entry->d_name);

        // read cpairs
        strcpy(filename, dir_src);          // make file path
        strcat(filename, entry->d_name);    // append file name onto directory

        stream_prop->cpairs_tracks[track] = read_cpairs_from_binary(filename);

        // make histogram
        float bin_width, bin_min, bin_max;
        bin_width = 50;
        bin_min = 0;
        bin_max = stream_prop->cpairs_tracks[track]->duration_sec * 1e3;
        stream_prop->hgrams[track] = init_hgram(bin_width, bin_min, bin_max);

        track++;
    }

    if (closedir(dir) == -1) {
        perror("Failed to close directory");
        return NULL;
    }

    return stream_prop;
}