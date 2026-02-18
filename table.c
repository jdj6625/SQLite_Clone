#include "table.h"
#include <stdlib.h>
#include <math.h>

void* rowSlot(Table* table, const uint32_t row_num)
{
    const uint32_t page_num = row_num/ROWS_PER_PAGE;
    void* page = table->pages[page_num];
    if (page == nullptr)
    {
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }

    const uint32_t row_offset = row_num % ROWS_PER_PAGE;
    const uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

Table* newTable()
{
    Table* table = malloc(sizeof(Table));
    table->num_rows = 0;
    for (uint32_t i=0; i<TABLE_MAX_PAGES; i++)
    {
        table->pages[i] = nullptr;
    }
    return table;
}

void freeTable(Table* table)
{
    for (uint32_t i=0; table->pages[i]; i++)
    {
        free(table->pages[i]);
    }
    free(table);
}
