#include "fft/hashing.h"
#include "debugging/asserts.h"
#include "fft/constellation.h"
#include <stdlib.h>

cpairs_t* init_cpairs(size_t n_cpairs) {

	cpairs_t* cpairs = (cpairs_t*)malloc(sizeof(cpairs_t));
	muse_assert_valid_ptr(cpairs, "failed to allocate memory for cpairs_t");

	cpairs->cpairs = (cpair_t*)malloc(sizeof(cpair_t) * n_cpairs);
	muse_assert_valid_ptr(cpairs->cpairs, "failed to allocate memory for array of cpair_t");

	// populate metadata
	cpairs->n_cpairs = n_cpairs;

	return cpairs;

}

cpairs_t* make_cpairs(cmap_t* cmap, double tzone_delta_freq_Hz, double tzone_delta_time_sec, double hop_freq_Hz, double hop_time_sec, double f) {

	// determine target zone width in samples ensuring it is at least 1
	unsigned int  tzone_delta_time_samples = max(1, tzone_delta_time_sec / cmap->cmap_delta_time_sec);

	// determine target zone height in samples ensuring it is at least 1
	unsigned int tzone_delta_freq_samples = max(1, tzone_delta_freq_Hz / cmap->cmap_delta_freq_Hz);

	// ensure window dimensions are odd for well defined centre anchor point
	if (tzone_delta_time_samples % 2 != 0) tzone_delta_time_samples++;
	if (tzone_delta_freq_samples % 2 != 0) tzone_delta_freq_samples++;
	
	unsigned int n_tzone_pnts = tzone_delta_time_samples * tzone_delta_freq_samples;

	// convert hop distances to samples
	// note: note hop has resolution up to window size chosen in cmap creation
	unsigned int hop_time_samples = max(1, hop_time_sec / cmap->cmap_delta_time_sec);
	unsigned int hop_freq_samples = max(1, hop_freq_Hz / cmap->cmap_delta_freq_Hz);
	
	// determine number of target windows that can fit inside constellation map
	unsigned int n_tzone_time_stacks = (cmap->n_stacks_seg - tzone_delta_time_samples) / hop_time_samples;
	unsigned int n_tzone_freq_stacks = (cmap->n_stacks_freq - tzone_delta_freq_samples) / hop_freq_samples;

	// determine number of points per target zone based on fan-out fraction, f
	// ensure atleast two points since one will be the anchor point
	//assert(f <= 1 && f > 0);
	//unsigned int tzone_fanout = min(2, (1 / f) * n_tzone_pnts);
	// subtract one to remove anchor point
	unsigned int tzone_fanout = n_tzone_pnts - 1;				// <- TODO

	size_t n_cpairs = n_tzone_time_stacks * n_tzone_freq_stacks * tzone_fanout;
	cpairs_t* cpairs = init_cpairs(n_cpairs);

	// populate cpairs metadata
	cpairs->max_freq_Hz = cmap->max_freq_Hz;
	cpairs->duration_sec = cmap->max_seg_sec;

	double delta_time_ms = cmap->delta_time_sec * 1e3;

	// populate cpairs
	size_t cpair_index = 0;
	for (size_t tzone_time = 0; tzone_time < n_tzone_time_stacks; tzone_time++) {
		for (size_t tzone_freq = 0; tzone_freq < n_tzone_freq_stacks; tzone_freq++) {

			// determine start of window
			size_t tzone_time_offset = tzone_time * hop_time_samples;
			size_t tzone_freq_offset = tzone_freq * hop_freq_samples;

			// determine anchor point
			size_t anchor_time = tzone_time_offset + (hop_time_samples / 2);
			size_t anchor_freq = tzone_freq_offset + (hop_freq_samples / 2);

			// loop over points in taget zone and form pairs
			for (size_t target_time = tzone_time_offset; target_time < tzone_delta_time_samples + tzone_time_offset; target_time++) {
				for (size_t target_freq = tzone_freq_offset; target_freq < tzone_delta_freq_samples + tzone_freq_offset; target_freq++) {

					// skip anchor point
					if (target_time == anchor_time && anchor_freq == target_freq)
						continue;

					cpairs->cpairs[cpair_index].hash = (hash_t){
						.freq1_Hz = cmap->freqs[anchor_time][anchor_freq] * cmap->delta_freq_Hz,
						.freq2_Hz = cmap->freqs[target_time][target_freq] * cmap->delta_freq_Hz,
						.delta_t_ms = (cmap->times[target_time][target_freq] - cmap->times[anchor_time][anchor_freq]) * delta_time_ms,
					};

					cpairs->cpairs[cpair_index++].anchor_time_ms = cmap->times[anchor_time][anchor_freq] * delta_time_ms;

				}
			}


		}
	}

	return cpairs;

}