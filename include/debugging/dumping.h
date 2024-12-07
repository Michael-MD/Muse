#pragma once
#include <stdint.h>

// forward declaration
typedef struct cpairs_t cpairs_t;

void dump_1d_arrayf32(float* data, size_t length, const char* filename);
void dump_1d_arrayu32(uint32_t* data, size_t length, const char* filename);

void dump_2d_arrayf32(float** data, const size_t length_rows, const size_t length_cols, const char* filename);
void dump_2d_arrayu32(uint32_t** data, const size_t length_rows, const size_t length_cols, const char* filename);

void dump_cpairs_to_file(cpairs_t* cpairs, const char* filename);