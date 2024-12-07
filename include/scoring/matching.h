#pragma once
#include <stdbool.h>

// forward declaration
typedef struct cpairs_t cpairs_t;
typedef struct hgram_t hgram_t;
typedef struct hash_t hash_t;

void update_hgram(hgram_t* hgram, cpairs_t* cpairs_track, cpairs_t* cpairs_clip, double freq_tol_Hz, double time_tol_ms);