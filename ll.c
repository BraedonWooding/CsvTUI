/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#include "table.h"

#include <stdlib.h>

/*

    [1, 2, 3, 4]
    realloc() <=> malloc, memcpy, free

 */

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
    if (row == 0 || row > table->row_len) return;

    LL *cur = table->head;
    for (int i = 1; i < row && cur; i++) cur = cur->next;

    if (cur && cur->next) {
        LL *remove = cur->next;
        cur->next = cur->next->next;
        free(remove);
    }
}

void table_insert_row(Table *table, int after, CsvRecord record) {
    LL *new = malloc(sizeof(*new));
    new->data = record;
    if (after == 0) {
        new->next = table->head;
        if (!table->head) table->tail = new;
        table->head = new;
    } else {
        LL *cur = table->head;
        for (int i = 1; i < after && cur; i++) cur = cur->next;
        if (cur == NULL || cur->next == NULL) {
            // a -> b -> c -> d
            //                ^tail
            new->next = NULL;
            table->tail->next = new;
            table->tail = new;
        } else {
            // a -> b -> d -> c
            new->next = cur->next;
            cur->next = new;
        }
    }
}

Table *table_init(void) {
    return calloc(1, sizeof(Table));
}

void table_apply(Table *table, map_fn fn, void *data) {
    for (LL *elem = table->head; elem != NULL; elem = elem->next) {
        fn(&elem->data, data);
    }
}
