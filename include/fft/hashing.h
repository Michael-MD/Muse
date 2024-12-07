#pragma once
#include <stdint.h>

// forward declaration
typedef struct cmap_t cmap_t;

typedef struct hash_t {
	uint16_t freq1_Hz;
	uint16_t freq2_Hz;
	float delta_t_ms;
} hash_t;

typedef struct cpair_t {
	hash_t hash;
	float anchor_time_ms;
} cpair_t;

typedef struct cpairs_t {
	cpair_t* cpairs;
	size_t n_cpairs;
	double max_freq_Hz;
	double duration_sec;	// duration of corresponding audio in sec
} cpairs_t;

cpairs_t* init_cpairs(size_t n_cpairs);

cpairs_t* make_cpairs(cmap_t* cmap, double tzone_delta_freq_Hz, double tzone_delta_time_sec, double hop_freq_Hz, double hop_time_sec, double f);
