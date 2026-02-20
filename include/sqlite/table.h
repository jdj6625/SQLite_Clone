#ifndef SQLITE_TABLE_H
#define SQLITE_TABLE_H
#include <stdint.h>
#include "row.h"

constexpr uint32_t PAGE_SIZE = 4096;
constexpr uint32_t TABLE_MAX_PAGES = 100;
constexpr uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
constexpr uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
} Table;

void* rowSlot(Table* table, uint32_t row_num);
Table* newTable();
void freeTable(Table* table);

#endif //SQLITE_TABLE_H