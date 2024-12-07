#include "scoring/matching.h"
#include "scoring/histogram/histogram.h"
#include "fft/hashing.h"

#include "debugging/dumping.h"

static inline bool check_hash_match(hash_t* hash_a, hash_t* hash_b, double freq_tol_Hz, double time_tol_ms) {
    // Calculate the absolute difference for frequency 1
    double freq1_diff = hash_a->freq1_Hz >= hash_b->freq1_Hz
        ? hash_a->freq1_Hz - hash_b->freq1_Hz
        : hash_b->freq1_Hz - hash_a->freq1_Hz;

    // Calculate the absolute difference for frequency 2
    double freq2_diff = hash_a->freq2_Hz >= hash_b->freq2_Hz
        ? hash_a->freq2_Hz - hash_b->freq2_Hz
        : hash_b->freq2_Hz - hash_a->freq2_Hz;

    // Calculate the absolute difference for the time delta
    double time_diff = hash_a->delta_t_ms >= hash_b->delta_t_ms
        ? hash_a->delta_t_ms - hash_b->delta_t_ms
        : hash_b->delta_t_ms - hash_a->delta_t_ms;

    // Check if the differences are within the specified tolerances
    bool freq1_match = freq1_diff <= freq_tol_Hz;
    bool freq2_match = freq2_diff <= freq_tol_Hz;
    bool time_match = time_diff <= time_tol_ms;

    // Return true if all fields match within tolerance, false otherwise
    return freq1_match && freq2_match && time_match;
}

void update_hgram(hgram_t* hgram, cpairs_t* cpairs_track, cpairs_t* cpairs_clip, double freq_tol_Hz, double time_tol_ms) {

	for (size_t track = 0; track < cpairs_track->n_cpairs; track++) {
		for (size_t clip = 0; clip < cpairs_clip->n_cpairs; clip++) {

			hash_t hash_track = cpairs_track->cpairs[track].hash;
			hash_t hash_clip = cpairs_clip->cpairs[clip].hash;

			// check if hashes match within some tollerance
			bool hashes_match = check_hash_match(&hash_track, &hash_clip, freq_tol_Hz, time_tol_ms);

			if (!hashes_match)
				continue;

			// enter match into histogram
			float anchor_time_track_ms = cpairs_track->cpairs[track].anchor_time_ms;
			float anchor_time_clip_ms = cpairs_clip->cpairs[clip].anchor_time_ms;

			hgram_add(hgram, anchor_time_track_ms - anchor_time_clip_ms);

		}

        if(track % 10000 == 0)
            dump_1d_arrayu32(hgram->bins, hgram->n_bins, "C:/Users/61481/Documents/code/sample music/dump/hgram/hgram.txt");
	}
}