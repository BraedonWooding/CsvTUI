/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#ifndef CSV_RECORD_H
#define CSV_RECORD_H

#include "defs.h"

/**
 * Returns all the csv headers.
 **/
const char* const *csv_get_headers(const Csv *csv);

/**
 * Initialise a new csv object.
 **/
Csv *csv_init(const char *headers);

void csv_print(Csv *csv);

#endif // CSV_RECORD_H