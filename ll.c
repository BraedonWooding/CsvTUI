/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#include "table.h"

#include <stdlib.h>

typedef struct ll_t LL;

struct ll_t {
    LL *next;
    CsvRecord data;
};

struct table_t {
    LL *head;
    LL *tail;
    int row_len;
};

CsvRecord *table_index(Table *table, int row) {
    LL *elem = table->head;
    for (int i = 1; elem != NULL && i < row; elem = elem->next, i++) { }
    return elem ? &elem->data : NULL;
}

void table_remove_row(Table *table, int row) {

}

void table_insert_row(Table *table, int after, CsvRecord record) {
}

Table *table_init(void) {
    return calloc(1, sizeof(Table));
}

void table_apply(Table *table, map_fn fn, void *data) {
    for (LL *elem = table->head; elem != NULL; elem = elem->next) {
        fn(&elem->data, data);
    }
}
