/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#include "table.h"

#include <stdlib.h>

typedef struct ll_t LL;

struct ll_t {
    LL *next;
    LL *prev;
    CsvRecord data;
};

struct table_t {
    LL *head;
    LL *tail;
    size_t row_len;
};

CsvRecord *table_index(Table *table, size_t row) {
    LL *elem = table->head;
    for (size_t i = 1; elem != NULL && i < row; elem = elem->next, i++) { }
    return elem ? &elem->data : NULL;
}

CsvRecord *table_remove_row(Table *table, size_t row) {
    return NULL;
}

void table_insert_row(Table *table, size_t after, CsvRecord record) {
    return NULL;
}

Table *table_init(void) {
    return calloc(1, sizeof(Table));
}

void table_apply(Table *table, map_fn fn, void *data) {
    for (LL *elem = table->head; elem != NULL; elem = elem->next) {
        fn(&elem->data, data);
    }
}
