/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#include <string.h>

#include "defs.h"
#include "csv_reader.h"
#include "csv.h"
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
    CsvSelected *selected = data;

    text_set(SET_BOLD, selected->row == record->row ? FG_WHITE : FG_BLACK,
             selected->row == record->row ? BG_BLACK : BG_WHITE);
    printf("% 3d", record->row);
    CLEAR_MODIFIERS();

    // each col is going to be black + white and have a length of 15
    for (CsvRecordCol *col = record->cols; col < record->cols + record->num_cols; col++) {
        text_set(SET_RESET, selected->row == record->row && selected->col == col->col ? FG_BLACK : FG_WHITE,
                selected->row == record->row && selected->col == col->col ? BG_WHITE : BG_BLACK);
        switch (col->type) {
            case CSV_RECORD_NUMBER: {
                printf("% 15f", col->_number);
                break;
            }
            case CSV_RECORD_TEXT: {
                printf("%15s", col->_text);
                break;
            }
        }
        // printf("%15s", col->_text);
        CLEAR_MODIFIERS();
    }
    printf("\n");

    // TODO: Print more cols/rows?
    //       Think about the extra arg we aren't using.
}

void csv_print(Csv *csv, CsvSelected *selected) {
    system(CLEAR_SCREEN);
    // print out cols
    // for the '0' col
    text_set(SET_BOLD, FG_BLACK, BG_WHITE);
    printf("   ");
    CLEAR_MODIFIERS();

    for (char **headers = csv->headers; *headers; headers++) {
        int header_len = strlen(*headers);
        if (header_len > 15) {
            // trim it
            header_len = 15;
        }

        text_set(SET_BOLD, selected->col == headers - csv->headers ? FG_WHITE : FG_BLACK, selected->col == headers - csv->headers ? BG_BLACK : BG_WHITE);

        // if we have it so it can't be even add more spaces on the left side
        for (int i = 0; i < (15 - header_len + 1) / 2; i++) putchar(' ');
        printf("%.*s", header_len, *headers);
        for (int i = 0; i < (15 - header_len) / 2; i++) putchar(' ');

        CLEAR_MODIFIERS();
    }

    printf("\n");
    table_apply(csv->table, print_record, selected);
}
