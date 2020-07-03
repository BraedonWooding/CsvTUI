#include "csv_reader.h"

#include <string.h>
#include <stdlib.h>

#include "csv.h"
#include "defs.h"
#include "table.h"

#define FILE_BUF_SIZE 1024

bool init_reader_file(CsvReader *reader, FILE *file, CsvConfig config) {
    memset(reader, 0, sizeof(*reader));
    reader->_read_mode.file = file;
    reader->config = config;

    reader->config.flags &= ~CSV_FLAGS_STRING_MODE;
    if (reader->config.flags & CSV_FLAGS_USE_DEFINED_HEADER) {
        reader->csv_headers = reader->config.headers;
    }
    return true;
}

bool init_reader_string(CsvReader *reader, const char *str_buf, CsvConfig config) {
    return false;
}

Csv *read_out_csv(CsvReader *reader, bool has_headers) {
    if (reader->config.flags & CSV_FLAGS_STRING_MODE) {
        printf("ERR: We don't support strings yet\n");
        return NULL;
    }

    char *headers = reader->csv_headers;
    char line_buf[FILE_BUF_SIZE];
    if (has_headers) {
        fgets(line_buf, FILE_BUF_SIZE, reader->_read_mode.file);
        headers = strdup(line_buf);
        int len = strlen(headers);
        if (headers[len - 1] == '\n') headers[len - 1] = '\0';
    }

    Csv *csv = csv_init(headers);
    int count = 0;
    while (fgets(line_buf, FILE_BUF_SIZE, reader->_read_mode.file) != NULL) {
        char *curCol = line_buf;
        while (*curCol == ' ' || *curCol == '\t') curCol++;
        if (*curCol == '\n' && (reader->config.flags & CSV_FLAGS_SKIP_EMPTY_RECORDS)) {
            continue;
        }
        int len = strlen(curCol);
        if (curCol[len - 1] == '\n') {
            curCol[len - 1] = '\0';
            len--;
        }
        if (len == csv->headers_length - 1 &&
            (reader->config.flags & CSV_FLAGS_SKIP_EMPTY_RECORDS) == CSV_FLAGS_SKIP_EMPTY_RECORDS) {
            continue;
        }

        CsvRecordCol *cols = malloc(sizeof(*cols) * csv->headers_length);
        int col = 0;

        for (char *next; curCol; curCol = next) {
            if (*curCol == ',') curCol++;
            next = strstr(curCol, ",");
            char *end;
            double tryNum = strtod(curCol, &end);
            // a20
            // ^

            while (end != curCol && end && (*end == ' ' || *end == '\t')) {
                end++;
            }

            if ((end == curCol || !end || (*end != '\0' && *end != ',')) || (reader->config.flags & CSV_FLAGS_ALL_STRINGS) == CSV_FLAGS_ALL_STRINGS) {
                cols[col].type = CSV_RECORD_TEXT;
                char *end = next ? next : strlen(curCol) + curCol;
                cols[col]._text = malloc(end - curCol + 1); // \0
                memcpy(cols[col]._text, curCol, end - curCol);
                cols[col]._text[end - curCol] = '\0';
            } else {
                cols[col].type = CSV_RECORD_NUMBER;
                cols[col]._number = tryNum;
            }

            cols[col].col = col;
            col++;
        }

        CsvRecord record = { .row = count + 1, .cols = cols, .num_cols = col };
        table_insert_row(csv->table, count, record);
        count++;
    }

    return csv;
}

