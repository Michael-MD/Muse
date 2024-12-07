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
 * 3. Writes the processed characteristic pairs to a binary file (`filename_dst`).
 *
 * @param filename_src The path to the source WAV file to be imported.
 * @param filename_dst The path to the destination binary file where the characteristic pairs
 *                      will be written.
 */
void archive_wav_track(char* filename_src, char* filename_dst);

/**
 * Archives all WAV files in the specified source directory and saves them
 * to the destination directory as binary files.
 *
 * This function opens the source directory, iterates through the files, and
 * processes each WAV file by changing its extension to `.bin` and archiving
 * it to the destination directory using the `archive_wav_track` function.
 *
 * The function assumes that the source and destination directories are valid
 * and that the filenames in the directory are valid WAV files.
 *
 * The `archive_wav_track` function is called for each WAV file, which processes
 * and stores the contents of the WAV file into a binary format.
 *
 * Parameters:
 * - dir_src: The path to the source directory containing WAV files.
 * - dir_dst: The path to the destination directory where the binary files
 *   will be saved.
 *
 * Returns:
 * - This function does not return any value. If there is an error opening
 *   or closing the directory, it will print an error message to `stderr`.
 *
 * Notes:
 * - It assumes the provided directory paths are valid and accessible.
 * - The file extension of each WAV file is changed to `.bin` before
 *   archiving.
 */
void archive_wav_dir(char* dir_src, char* dir_dst);