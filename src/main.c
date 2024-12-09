#include "stream/live_recognition.h"
#include "binary/archiving.h"

#define ms * 1e-3

#include <dirent.h>
#include <argtable3.h>
#include <getopt.h>


int main(int argc, char* argv[]) {
    // Argument definitions
    struct arg_lit* help = arg_lit0("h", "help", "Display this help message");
    struct arg_str* start_listening = arg_strn("l", "startListening", "<src_dir>", 0, 1,
        "Begins listening through microphone for audio using directory containing archived tracks.");
    struct arg_str* archive_tracks = arg_strn("a", "archiveTracks", "<src_dir> <dst_dir>", 0, 2,
        "Archives directory of .wav tracks to be used for music recognition.");
    struct arg_end* end = arg_end(20);  // Marks the end of the argument table

    // Argument table setup
    void* argtable[] = { help, start_listening, archive_tracks, end };

    // Parse command-line arguments
    int nerrors = arg_parse(argc, argv, argtable);

    // Handle parsing errors
    if (nerrors > 0) {
        // Print the error message and syntax guide
        arg_print_errors(stdout, end, argv[0]);
        printf("\nUse the --help option for more information.\n");
        return 1;
    }

    // Display help message if requested
    if (help->count > 0) {
        printf("Usage:\n");
        arg_print_syntax(stdout, argtable, "\n");
        printf("\nOptions:\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        return 0;
    }

    // Handle the 'startListening' option
    if (start_listening->count > 0) {

        if (start_listening->count != 1) {
            printf("Error: '--startListening' expects a single argument representing the source directory of archived tracks. "
                "Received %i argument(s).\n", start_listening->count);
            return 1;
        }

        char* dir_src = start_listening->sval[0];  // Get the source directory for archived tracks

        // Define sample rate and frames per buffer for live recognition
        double sample_rate = 44100.0;  // Standard audio sample rate (44.1 kHz)
        unsigned long frames_per_buffer_sec = 2;  // Number of frames to process per buffer (2 seconds)

        printf("Starting live recognition using directory: %s\n", dir_src);
        begin_live_recognition(sample_rate, frames_per_buffer_sec, dir_src);  // Call live recognition function
    }

    // Handle the 'archiveTracks' option
    if (archive_tracks->count > 0) {

        if (archive_tracks->count != 2) {
            printf("Error: '--archiveTracks' expects two arguments: the source directory containing .wav files and the "
                "destination directory for the binary files. Received %i argument(s).\n", archive_tracks->count);
            return 1;
        }

        char* dir_src = archive_tracks->sval[0];  // Source directory of .wav files
        char* dir_dst = archive_tracks->sval[1];  // Destination directory for archived binary files

        printf("Archiving .wav files from '%s' to '%s'.\n", dir_src, dir_dst);
        archive_wav_dir(dir_src, dir_dst);  // Call function to archive .wav files
        printf("Done.\n");
    }

    // Clean up argument table resources
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

    return 0;
}