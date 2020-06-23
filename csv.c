/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#include <string.h>

#include "defs.h"
#include "csv_reader.h"
#include "csv_record.h"
#include "table.h"
#include "color.h"

char **const csv_get_headers(const Csv *csv) {
    return csv->headers;
}

Csv *csv_init(const char *headers) {
    Csv *csv = malloc(sizeof(*csv));
    csv->table = table_init();
    csv->headers_length = 0;
    const char *end;

    // count ',' in the headers
    for (end = headers; *end; end++) {
        if (*end == ',') csv->headers_length++;
    }

    // a,b => 2 but "" => 0
    // important distinction to allow us to create new csv files.
    if (csv->headers_length > 0 || end != headers) csv->headers_length++;

    // null terminating header
    csv->headers = malloc(sizeof(*csv->headers) * (csv->headers_length + 1));
    csv->headers[csv->headers_length] = NULL;

    // loop through and create the headers by copying the section
    // from the last ',' to the next one.
    const char *prev = headers;
    int col_count = 0;

    // think about how this code will be written by the compiler
    /*
    for:
        if (cur > end) goto end;
        if (*cur != ',' && *cur != '\0') goto for;
            ...
        goto for;
    end:
    */
    for (const char *cur = headers; cur <= end; cur++) {
        if (*cur == ',' || *cur == '\0') {
            csv->headers[col_count] = malloc(cur - prev + 1);
            strncpy(csv->headers[col_count], prev, cur - prev);
            csv->headers[col_count][cur - prev] = 0;

            prev = cur + 1; // skip past any ',' or '\0'
            col_count++;
        }
    }

    return csv;
}

static void print_record(CsvRecord *record, void *data) {
    text_set(SET_BOLD, FG_BLACK, BG_WHITE);
    printf("% 3d", record->row);
    CLEAR_MODIFIERS();

    text_set(SET_RESET, FG_WHITE, BG_BLACK);
    // each col is going to be black + white and have a length of 15
    for (CsvRecordCol *col = record->cols; col < record->cols + record->num_cols; col++) {
        
    }
    printf("\n");
    CLEAR_MODIFIERS();

    // TODO: Print more cols/rows?
    //       Think about the extra arg we aren't using.
}

void csv_print(Csv *csv) {
    system(CLEAR_SCREEN);
    // print out cols
    text_set(SET_BOLD, FG_BLACK, BG_WHITE);
    // for the '0' col
    printf("   ");

    for (char **headers = csv->headers; *headers; headers++) {
        int header_len = strlen(*headers);
        if (header_len > 15) {
            // trim it
            header_len = 15;
        }

        // if we have it so it can't be even add more spaces on the left side
        for (int i = 0; i < (15 - header_len + 1) / 2; i++) putchar(' ');
        printf("%.*s", header_len, *headers);
        for (int i = 0; i < (15 - header_len) / 2; i++) putchar(' ');
    }

    printf("\n");
    table_apply(csv->table, print_record, NULL);
}
