
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "fft/hashing.h"
#include "binary/binary_io.h"


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