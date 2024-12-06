#pragma once
#include "audio/audio.h"
#include <sndfile.h>

/**
 * @brief Reads a WAV file and loads its audio data into an audio structure.
 *
 * This function opens and reads the specified WAV file, extracting the audio data and storing it in
 * the provided `audio_t` structure. The structure should be initialized before calling this function.
 * However, audio->samples is assumed uninitialized.
 *
 * @param filename The path to the WAV file to be read.
 * @param audio A pointer to an `audio_t` structure that will hold the audio data from the file.
 *
 * @return void
 */
void read_wav_file(char* filename, audio_t* audio);


/**
 * @brief Converts the audio track to mono.
 *
 * This function processes the audio data in the provided `audio_t` structure and converts it into a
 * single channel (mono). The original stereo or multi-channel audio data will be mixed down to mono.
 *
 * @param audio A pointer to an `audio_t` structure containing the audio data to be converted.
 *
 * @return void
 */
void make_audio_mono(audio_t* audio);


/**
 * @brief Imports an audio track from a WAV file and converts it to mono.
 *
 * This function reads an audio track from the specified WAV file, loads the audio data into an
 * `audio_t` structure, and converts the track to mono. The structure should be initialized before
 * calling this function.
 *
 * @param filename The path to the WAV file to be imported.
 * @param audio A pointer to an `audio_t` structure that will hold the mono audio data from the file.
 *
 * @return void
 */
void import_audio_track_mono(char* filename, audio_t* audio);
