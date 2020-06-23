/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#ifndef DEFS_H
#define DEFS_H

#include <stddef.h>
#include <stdlib.h>

#ifdef _WIN32
#   define WINDOWS_COMPATIBILITY
#   define CLEAR_SCREEN "cls"
#else
#   define UNIX_COMPATIBILITY
#   define CLEAR_SCREEN "clear"
#endif

typedef struct table_t Table;

typedef struct csv_t {
    // NOTE: in C we rarely will #include other .h files
    //       to just get a pointer definition
    Table *table;

    char **headers;
    int headers_length;
} Csv;

typedef struct csv_record_col_t {
    /**
     * The inner possible data types for this record.
     */
    union {
        double _number;
        char  *_text;
    };

    /**
     * The type that is held by the anon union.
     */
    enum csv_record_type_t {
        CSV_RECORD_NUMBER,
        CSV_RECORD_TEXT,
    } type;

    int col;
} CsvRecordCol;

typedef struct csv_record_t {
    CsvRecordCol *cols;
    int row;
    int num_cols;
} CsvRecord;

typedef void(*map_fn)(CsvRecord *record, void *data);

/**
 * Find length of 0 terminated memory section.
 **/
inline int memlen(const void* const *base) {
    const void * const *cur;
    for (cur = base; *cur != NULL; cur++) {}
    return cur - base;
}

int getch(void);

#endif // DEFS_H