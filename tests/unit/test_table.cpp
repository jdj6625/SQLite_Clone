#include <gtest/gtest.h>

extern "C" {
#include "sqlite/table.h"
}

TEST(Table, TableCreation)
{
   Table* table = newTable();
   ASSERT_NE(table, nullptr);

   EXPECT_EQ(table->num_rows, 0);
   for (auto & page : table->pages)
   {
      EXPECT_EQ(page, nullptr);
   }
   freeTable(table);
}

TEST(Table, FreeTable_DoesNotCrash) {
   Table* t = newTable();
   ASSERT_NE(t, nullptr);

   for (auto& page : t->pages) page = nullptr;
   t->pages[0] = std::malloc(PAGE_SIZE);
   ASSERT_NE(t->pages[0], nullptr);
   t->pages[5] = std::malloc(PAGE_SIZE);
   ASSERT_NE(t->pages[5], nullptr);

   freeTable(t);
}

// 1) Same row -> same pointer
TEST(RowSlot, SameRowSamePointer) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    void* p1 = rowSlot(t, 0);
    void* p2 = rowSlot(t, 0);

    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    EXPECT_EQ(p1, p2);

    freeTable(t);
}

// 2) Adjacent rows in same page -> pointers differ by ROW_SIZE
TEST(RowSlot, AdjacentRowsHaveRowSizeStride) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    uint8_t* p0 = static_cast<uint8_t*>(rowSlot(t, 0));
    uint8_t* p1 = static_cast<uint8_t*>(rowSlot(t, 1));

    ASSERT_NE(p0, nullptr);
    ASSERT_NE(p1, nullptr);

    EXPECT_EQ(reinterpret_cast<uintptr_t>(p1) - reinterpret_cast<uintptr_t>(p0),
              static_cast<uintptr_t>(ROW_SIZE));

    freeTable(t);
}

// 3) Boundary between pages -> different pages, correct stride
TEST(RowSlot, CrossingPageBoundaryUsesNewPage) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    const uint32_t last_in_page0 = ROWS_PER_PAGE - 1;
    const uint32_t first_in_page1 = ROWS_PER_PAGE;

    uint8_t* a = static_cast<uint8_t*>(rowSlot(t, last_in_page0));
    uint8_t* b = static_cast<uint8_t*>(rowSlot(t, first_in_page1));

    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);

    // They should not be the same pointer.
    EXPECT_NE(a, b);

    // a should be within page 0 at offset (ROWS_PER_PAGE-1)*ROW_SIZE
    // b should be within page 1 at offset 0
    // We can also directly check which pages got allocated:
    ASSERT_NE(t->pages[0], nullptr);
    ASSERT_NE(t->pages[1], nullptr);
    EXPECT_NE(t->pages[0], t->pages[1]);

    freeTable(t);
}

// 4) Lazy allocation: touching one row should allocate only that page
TEST(RowSlot, LazyAllocatesOnlyNeededPage) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    // Before: all pages should be null (assuming newTable initializes them)
    // We'll at least check a few.
    EXPECT_EQ(t->pages[0], nullptr);
    EXPECT_EQ(t->pages[1], nullptr);

    // Touch a row that lands on page 2
    const uint32_t row_on_page2 = 2 * ROWS_PER_PAGE; // first row in page 2
    void* p = rowSlot(t, row_on_page2);
    ASSERT_NE(p, nullptr);

    EXPECT_EQ(t->pages[0], nullptr);
    EXPECT_EQ(t->pages[1], nullptr);
    ASSERT_NE(t->pages[2], nullptr);

    freeTable(t);
}

// 5) Write-through: writing to the returned slot persists for the same row
TEST(RowSlot, WriteThroughPersistsForSameRow) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    const uint32_t row = 7;
    uint8_t* slot1 = static_cast<uint8_t*>(rowSlot(t, row));
    ASSERT_NE(slot1, nullptr);

    // Write a recognizable pattern into the row region
    std::memset(slot1, 0xAB, ROW_SIZE);

    uint8_t* slot2 = static_cast<uint8_t*>(rowSlot(t, row));
    ASSERT_NE(slot2, nullptr);
    EXPECT_EQ(slot1, slot2);

    // Verify the bytes are still there
    for (size_t i = 0; i < ROW_SIZE; ++i) {
        EXPECT_EQ(slot2[i], 0xAB) << "Mismatch at byte " << i;
        if (slot2[i] != 0xAB) break; // optional: reduce spam
    }

    freeTable(t);
}

// 6) No overlap: writing one row should not change another row in same page
TEST(RowSlot, DifferentRowsDoNotOverlap) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    const uint32_t rowA = 3;
    const uint32_t rowB = 4; // same page, adjacent

    uint8_t* a = static_cast<uint8_t*>(rowSlot(t, rowA));
    uint8_t* b = static_cast<uint8_t*>(rowSlot(t, rowB));

    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);

    // Fill both rows with different patterns
    std::memset(a, 0x11, ROW_SIZE);
    std::memset(b, 0x22, ROW_SIZE);

    // Re-check each region stayed intact
    for (size_t i = 0; i < ROW_SIZE; ++i) {
        EXPECT_EQ(a[i], 0x11) << "Row A corrupted at byte " << i;
        if (a[i] != 0x11) break;
    }
    for (size_t i = 0; i < ROW_SIZE; ++i) {
        EXPECT_EQ(b[i], 0x22) << "Row B corrupted at byte " << i;
        if (b[i] != 0x22) break;
    }

    freeTable(t);
}