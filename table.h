/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#ifndef TABLE_H
#define TABLE_H

#include "defs.h"

/**
 * Index a given row
 **/
CsvRecord *table_index(Table *table, size_t row);

/**
 * Remove a given row and return it.
 **/
CsvRecord *table_remove_row(Table *table, size_t row);

/**
 * Insert a row after the given index.
 * NOTE: 'after' is '1' indexed (corresponds to row) so '0' will be always inserting prior to first row
 **/
void table_insert_row(Table *table, size_t after, CsvRecord record);

/**
 * Initialise the table.
 */
Table *table_init(void);

/**
 * Map a function over all the records in the set.
 */
void table_apply(Table *table, map_fn fn, void *data);

#endif // TABLE_H