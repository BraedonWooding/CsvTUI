/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "csv_reader.h"
#include "csv.h"
#include "defs.h"

#define ERR(...) do { \
    fprintf(stderr, "[Error] " __VA_ARGS__); \
    usage(); \
    exit(1); \
} while(0)

void usage(void) {
    printf(
        "./csv [ -f file.csv ] [ -s \"<Text>\" ] [ --skip-empty=LINES|RECORDS ] [ -h=\"a,b,c,d\" ] [ --force-type=STRING ]\n\n"
        "-f [file] signifies to open the csv reader on a file\n"
        "-s \"<Text>\" read from the given command line argument\n"
        "--skip-empty=LINES|RECORDS enables you to skip empty lines or records i.e. --skip-empty=LINES\n"
        "-h \"a,b,c,d\" provide a set of headers\n"
        "--force-type=STRING allows you to force all the type of all record columns to be string\n"
    );
}

int main(int argc, char *argv[]) {
    enum csv_flags_t flags = CSV_FLAGS_NONE;
    char *headers = NULL;
    char *input = NULL;
    FILE *file_input = NULL;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-f")) {
            if (file_input || input) ERR("Duplicate file and/or string input\n");
            if (i + 1 >= argc) ERR("Missing argument for -f\n");

            file_input = fopen(argv[i + 1], "r");
            if (!file_input) ERR("%s\n", strerror(errno));
            i++;
        } else if (!strcmp(argv[i], "-s")) {
            if (file_input || input) ERR("Duplicate file and/or string input\n");
            if (i + 1 >= argc) ERR("Missing argument for -s\n");
            input = argv[i + 1];
            i++;
        } else if (!strcmp(argv[i], "-h")) {
            if (headers) ERR("Duplicate headers\n");
            if (i + 1 >= argc) ERR("Missing argument for -h\n");
            headers = argv[i + 1];
            i++;
        } else if (!strncmp(argv[i], "--skip-empty", strlen("--skip-empty"))) {
            // if flags contains either records or lines
            if (flags & CSV_FLAGS_SKIP_EMPTY_RECORDS) ERR("Duplicate --skip-empty\n");

            char *arg = strstr(argv[i], "=");
            if (!arg || *arg != '=') ERR("Missing argument for --skip-empty\n");
            if (!strcmp(arg + 1, "LINES")) {
                flags |= CSV_FLAGS_SKIP_EMPTY_LINES;
            } else if (!strcmp(arg + 1, "RECORDS")) {
                flags |= CSV_FLAGS_SKIP_EMPTY_LINES;
            } else {
                ERR("Invalid argument %s\n", arg + 1);
            }
        } else if (!strncmp(argv[i], "--force-type", strlen("--force-type"))) {
            if (flags & CSV_FLAGS_ALL_STRINGS) ERR("Duplicate --force-type\n");

            char *arg = strstr(argv[i], "=");
            if (!arg || *arg != '=') ERR("Missing argument for --force-type\n");
            if (!strcmp(arg + 1, "STRING")) {
                flags |= CSV_FLAGS_ALL_STRINGS;
            } else {
                ERR("Invalid argument %s\n", arg + 1);
            }
        } else {
            ERR("Unknown argument %s\n", argv[i]);
        }
    }

    CsvReader reader;
    CsvConfig config = { .flags = flags, .headers = headers };

    if (file_input) {
        if (!init_reader_file(&reader, file_input, config)) ERR("Couldn't initialise reader\n");
    } else if (input) { 
        if (!init_reader_string(&reader, input, config)) ERR("Couldn't initialise reader\n");
    } else {
        // Possible extension allow opening an empty csv
        ERR("Needs to provide atleast one of '-s' or '-f' for now");
    }

    Csv *csv = read_out_csv(&reader, !!headers);
    if (!csv) ERR("Failure to read CSV %s", reader.err_buf);

    // print it out
    csv_print(csv);

    // TODO: TUI Mode
    // for (;;) {

    // }

    return 0;
}
