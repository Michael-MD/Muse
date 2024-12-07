#pragma once

// forward declaration
typedef struct cpairs_t cpairs_t;
typedef struct audio_t audio_t;

/**
 * @brief Imports a mono WAV audio file, processes it to extract characteristic pairs,
 *        and writes the resulting data to a binary file.
 *
 * This function performs the following steps:
 * 1. Imports the audio data from the provided source file (`filename_src`).
 * 2. Processes the audio data to generate characteristic pairs (cpairs).
 * 3. Writes the processed characteristic pairs to a binary file (`filename_dest`).
 *
 * @param filename_src The path to the source WAV file to be imported.
 * @param filename_dest The path to the destination binary file where the characteristic pairs
 *                      will be written.
 */
void archive_wav_track(char* filename_src, char* filename_dest);


void archive_wav_dir(char* filename_src, char* filename_dest);