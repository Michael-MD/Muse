#pragma once
#include <stdbool.h>

// forward declaration
typedef struct cpairs_t cpairs_t;
typedef struct hgram_t hgram_t;
typedef struct hash_t hash_t;

/**
 * Updates the histogram with the differences in start times between the control pairs of the track and clip.
 * This function compares each pair of hashes from the track and the clip, checking whether they match within
 * the specified tolerance in frequency and time. If the hashes match, it calculates the difference in start times
 * and updates the histogram accordingly.
 *
 * @param hgram The histogram to be updated with the time offsets between the track and the clip.
 * @param cpairs_track A set of control pairs representing the track data.
 * @param cpairs_clip A set of control pairs representing the clip data.
 * @param freq_tol_Hz The frequency tolerance in Hz for determining hash matches.
 * @param time_tol_ms The time tolerance in milliseconds for determining hash matches.
 *
 * @returns void
 */
void update_hgram(hgram_t* hgram, cpairs_t* cpairs_track, cpairs_t* cpairs_clip, double freq_tol_Hz, double time_tol_ms);

/**
 * Scores the histogram to determine if the peak count is sufficiently large, indicating a match between the track and clip.
 * The function compares the peak count in the histogram to the mean and variance of the histogram data.
 * If the peak count exceeds the mean by at least `n` standard deviations, it returns `true` to indicate a match.
 *
 * @param hgram The histogram containing the time differences between matched pairs.
 * @param n The number of standard deviations from the mean required for a valid match.
 *
 * @returns bool `true` if the peak count is sufficiently large (i.e., a match is found), otherwise `false`.
 */
bool score_hgram(hgram_t* hgram, unsigned int n);