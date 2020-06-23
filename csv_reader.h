/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#ifndef CSV_READER_H
#define CSV_READER_H

#include <stddef.h>
#include <stdio.h> // for FILE
#include <stdbool.h>

#include "defs.h"

#define READ_BUF_SIZE (1024)

typedef struct csv_config_t {
    /**
     * Represents a series of bitflags for config.
     **/
    enum csv_flags_t {
        CSV_FLAGS_NONE = 0,
        CSV_FLAGS_SKIP_EMPTY_LINES   = 2,      // skip any empty lines (else they'll be empty records)
        CSV_FLAGS_SKIP_EMPTY_RECORDS = 3,      // skipping empty records will also skip empty lines
        CSV_FLAGS_USE_DEFINED_HEADER = 1 << 2, // use the defined headers
        CSV_FLAGS_ALL_STRINGS        = 1 << 3, // force all data types to be strings
        CSV_FLAGS_STRING_MODE        = 1 << 4, // indicates that the reader was initialised with a string
    } flags;

    /**
     * The headers.
     **/
    char *headers;
} CsvConfig;

typedef struct csv_reader_t {
    /**
     * What read mode are we in?
     */
    union {
        FILE        *file_buf;
        const char  *read_buf;
    } _read_mode;

    /**
     * The headers read.
     */
    char       *csv_headers;

    /**
     * Total number of columns read.
     **/
    int      col_count;

    /**
     * Total number of rows read.
     **/
    int      row_count;

    /*
        The configuration for this csv reader.
     */
    CsvConfig   config;

    /*
        Buffer for error messages.
     */
    char        err_buf[256];
} CsvReader;

/**
 *  Initialise the reader from a file and use the given config.
 *  returns false (and prints out err to stderr) in event of error.
 *  which can include if you provide incorrect configuration.
 */
bool init_reader_file(CsvReader *reader, FILE *filename, CsvConfig config);

/**
 *  Initialise the reader from a file and use the given config.
 *  returns false (and prints out err to stderr) in event of error.
 *  which can include if you provide incorrect configuration.
 */
bool init_reader_string(CsvReader *reader, const char *str_buf, CsvConfig config);

/**
 *  Read a CSV file out from a reader.
 **/
Csv *read_out_csv(CsvReader *reader, bool has_headers);

#endif // CSV_READER_H
