/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#ifndef DEFS_H
#define DEFS_H

#include <stddef.h>

#ifdef _WIN32
#   define WINDOWS_COMPATIBILITY
#   define CLEAR_SCREEN "cls"
#else
#   define UNIX_COMPATIBILITY
#   define CLEAR_SCREEN "clear"
#endif

typedef struct table_t Table;
typedef struct csv_t Csv;

typedef void(*map_fn)(CsvRecord *record, void *data);

typedef struct csv_record_metadata_t {
    /**
     * What row the record is on
     * NOTE: This doesn't include any skipped records nor headers
     * 1 indexed.
     **/
    size_t row_no;

    /**
     * What column this record is on
     * i.e. the count of ',' + 1 prior to this.
     * 1 indexed.
     **/
    size_t col_no;
} CsvRecordMetadata;

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
} CsvRecordCol;

typedef struct csv_record_t {
    CsvRecordCol *cols;
    CsvRecordMetadata metadata;
} CsvRecord;

/**
 * Find length of 0 terminated memory section.
 **/
inline size_t memlen(const void* const *base) {
    void **cur;
    for (cur = base; *cur != NULL; cur++) {}
    return cur - base;
}

inline void clear_screen(void) {
    system(CLEAR_SCREEN);
}

#endif // DEFS_H