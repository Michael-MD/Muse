#pragma once

#include "debugging/dumping.h"
#include <stdio.h>
#include <stdint.h>

void dump_1d_arrayf32(float* data, size_t length, const char* filename) {
    
    // Open the file for writing (binary mode, or text mode depending on preference)
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write array contents to the file
    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%f\n", data[i]);
    }

    // Close the file
    fclose(file);
    printf("Array dumped to file: %s\n", filename);

}


void dump_1d_arrayu32(uint32_t* data, size_t length, const char* filename) {
    
    // Open the file for writing (binary mode, or text mode depending on preference)
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write array contents to the file
    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%u\n", data[i]);
    }

    // Close the file
    fclose(file);
    printf("Array dumped to file: %s\n", filename);
}

void dump_2d_arrayf32(float** data, const size_t length_rows, const size_t length_cols, const char* filename) {

    // Open the file for writing (binary mode, or text mode depending on preference)
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write array contents to the file
    for (size_t i = 0; i < length_rows; i++) {
        for (size_t j = 0; j < length_cols; j++) {
            fprintf(file, "%f\n", data[i][j]);

        }
    }

    // Close the file
    fclose(file);
    printf("Array dumped to file: %s\n", filename);

}

void dump_2d_arrayu32(uint32_t** data, const size_t length_rows, const size_t length_cols, const char* filename) {

    // Open the file for writing (binary mode, or text mode depending on preference)
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write array contents to the file
    for (size_t i = 0; i < length_rows; i++) {
        for (size_t j = 0; j < length_cols; j++) {
            fprintf(file, "%u\n", data[i][j]);

        }
    }

    // Close the file
    fclose(file);
    printf("Array dumped to file: %s\n", filename);

}