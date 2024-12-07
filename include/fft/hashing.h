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

/**
 * Initializes a `cpairs_t` structure, allocating memory for an array of `cpair_t`
 * structures to store coordinate pairs. The function takes the number of coordinate
 * pairs to be allocated and ensures that memory is successfully allocated for both
 * the `cpairs_t` structure and the array of `cpair_t` structures.
 *
 * @param n_cpairs The number of coordinate pairs (`cpair_t`) to allocate memory for.
 *                 This determines the size of the array of coordinate pairs.
 *
 * @return A pointer to a `cpairs_t` structure, which contains the allocated array
 *         of coordinate pairs (`cpair_t`). The structure also contains metadata
 *         related to the number of coordinate pairs (`n_cpairs`).
 *
 * @throws An assertion failure if memory allocation fails for either the `cpairs_t`
 *         structure or the array of coordinate pairs.
 */
cpairs_t* init_cpairs(size_t n_cpairs);

/**
 * Frees the memory allocated for a `cpairs_t` structure, including its array
 * of `cpair_t` elements. This function ensures that all dynamically allocated
 * memory is properly released to avoid memory leaks.
 *
 * @param cpairs Pointer to the `cpairs_t` structure to be freed. This structure contains
 *               the array of `cpair_t` elements.
 */
void free_cpairs(cpairs_t* cpairs);

/**
 * Creates a `cpairs_t` structure by generating coordinate pairs within a specified
 * target zone in a constellation map (`cmap`). The function calculates the appropriate
 * window sizes for frequency and time, then generates pairs of coordinates (excluding
 * the anchor point) within each target zone. These pairs are stored in the `cpairs_t`
 * structure for further processing. The function uses parameters such as frequency and
 * time hop sizes, along with the fan-out fraction, to determine the number of coordinate
 * pairs to be created.
 *
 * @param cmap Pointer to the `cmap_t` structure, which contains the constellation map
 *             with frequency-time data and metadata.
 * @param tzone_delta_freq_Hz The width of the target zone in the frequency domain,
 *                            in Hertz. This defines the horizontal size of the target
 *                            zone for coordinate pair generation.
 * @param tzone_delta_time_sec The width of the target zone in the time domain, in seconds.
 *                             This defines the vertical size of the target zone for
 *                             coordinate pair generation.
 * @param hop_freq_Hz The frequency hop size, in Hertz, used to determine how far the
 *                    target zone moves in the frequency domain.
 * @param hop_time_sec The time hop size, in seconds, used to determine how far the
 *                     target zone moves in the time domain.
 * @param f The fan-out fraction, which is used to determine how many coordinate pairs
 *          will be generated for each target zone. At least one pair is created for
 *          every target zone (excluding the anchor point).
 *
 * @return A pointer to a `cpairs_t` structure containing the generated coordinate pairs
 *         (`cpair_t`). This structure also includes metadata such as the maximum frequency
 *         and duration in the constellation map.
 *
 * @throws An assertion failure if any of the memory allocation or parameter validation
 *         conditions are not met (e.g., invalid zone sizes or hop sizes).
 */
cpairs_t* make_cpairs(cmap_t* cmap, double tzone_delta_freq_Hz, double tzone_delta_time_sec,
	double hop_freq_Hz, double hop_time_sec, double f);
