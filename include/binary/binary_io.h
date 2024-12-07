#pragma once

// forward declaration
typedef struct cpairs_t cpairs_t;

/**
 * Writes a cpairs_t structure to a binary file.
 *
 * This function takes a `cpairs_t` structure and writes it to a specified binary file. It first
 * sorts the cpairs by their hash values for faster access. Then, it writes the metadata, such as
 * the number of cpairs, maximum frequency, and duration of the corresponding audio, followed by
 * the individual `cpair_t` pairs, which include hash data and the offset in milliseconds for
 * each pair.
 *
 * In case of any error during writing, the function prints an error message and returns `-1`.
 * On successful completion, the function does not return any value.
 *
 * @param filename The name of the binary file to write to.
 * @param cpairs A pointer to the cpairs_t structure that contains the data to be written.
 */
void write_cpairs_to_binary(const char* filename, const cpairs_t* cpairs);


/**
 * Reads a cpairs_t structure from a binary file.
 *
 * This function reads a binary file containing a serialized `cpairs_t` structure. It first reads
 * the metadata (such as the number of cpairs, maximum frequency, and duration of the audio) and
 * then reads the `cpair_t` pairs, which consist of hash data and the offset in milliseconds for
 * each pair. The function dynamically allocates memory for the `cpairs` array within the `cpairs_t`
 * structure based on the number of pairs found in the file.
 *
 * In case of any error during reading or memory allocation, the function prints an error message,
 * frees any allocated memory, and returns `NULL`. If successful, the function returns a pointer to
 * the populated `cpairs_t` structure.
 *
 * @param filename The name of the binary file to read from.
 *
 * @return A pointer to the cpairs_t structure containing the read data, or `NULL` if an error occurred.
 */
cpairs_t* read_cpairs_from_binary(const char* filename);
