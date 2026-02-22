#include <gtest/gtest.h>

extern "C" {
#include "sqlite/input_processing.h"
}

static InputBuffer makeIB(const char* s) {
    InputBuffer ib{};
    const size_t n = std::strlen(s);
    ib.buffer = (char*)std::malloc(n + 1);
    std::memcpy(ib.buffer, s, n + 1);
    ib.buffer_length = n + 1;
    ib.input_length = n;
    return ib;
}

static void freeIB(InputBuffer* ib) {
    if (!ib) return;
    std::free(ib->buffer);
    ib->buffer = nullptr;
    ib->buffer_length = 0;
    ib->input_length = 0;
}

TEST(Smoke, PrintPrompt_DoesSomething) {
    testing::internal::CaptureStdout();
    printPrompt();
    const std::string out = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(out.empty());
}

TEST(Smoke, Getline_ReadsOneLine) {
    FILE* f = std::tmpfile();
    ASSERT_NE(f, nullptr);

    std::fputs("hello\n", f);
    std::rewind(f);

    char* line = nullptr;
    size_t cap = 0;
    const ssize_t n = getline(&line, &cap, f);

    EXPECT_GT(n, 0);
    EXPECT_NE(line, nullptr);

    std::free(line);
    std::fclose(f);
}

TEST(Smoke, PrepareStatement_Select) {
    InputBuffer ib = makeIB("select");
    Statement st{};

    const PrepareResult r = prepareStatement(&ib, &st);

    EXPECT_EQ(r, PREPARE_SUCCESS);
    EXPECT_EQ(st.type, STATEMENT_SELECT);

    freeIB(&ib);
}

TEST(Smoke, PrepareStatement_Insert_Basic) {
    InputBuffer ib = makeIB("insert 1 bob a@b.com");
    Statement st{};

    const PrepareResult r = prepareStatement(&ib, &st);

    EXPECT_EQ(r, PREPARE_SUCCESS);
    EXPECT_EQ(st.type, STATEMENT_INSERT);

    freeIB(&ib);
}

TEST(Smoke, ExecuteInsert_IncrementsRowCount) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    Statement st{};
    st.type = STATEMENT_INSERT;
    st.row_to_insert.id = 1;

    // keep it minimal: just ensure strings are non-garbage
    std::memset(st.row_to_insert.username, 0, USERNAME_SIZE);
    std::memset(st.row_to_insert.email, 0, EMAIL_SIZE);
    std::memcpy(st.row_to_insert.username, "bob", 3);
    std::memcpy(st.row_to_insert.email, "a@b.com", 6);

    const uint32_t before = t->num_rows;
    const ExecuteResult r = executeInsert(&st, t);

    EXPECT_EQ(r, EXECUTE_SUCCESS);
    EXPECT_EQ(t->num_rows, before + 1);

    freeTable(t);
}

TEST(Smoke, ExecuteSelect_Runs) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    // Insert one row so select has something to iterate
    Statement ins{};
    ins.type = STATEMENT_INSERT;
    ins.row_to_insert.id = 1;
    std::memset(ins.row_to_insert.username, 0, USERNAME_SIZE);
    std::memset(ins.row_to_insert.email, 0, EMAIL_SIZE);
    std::memcpy(ins.row_to_insert.username, "bob", 3);
    std::memcpy(ins.row_to_insert.email, "a@b.com", 6);
    ASSERT_EQ(executeInsert(&ins, t), EXECUTE_SUCCESS);

    Statement sel{};
    sel.type = STATEMENT_SELECT;

    // Don’t assert exact output formatting—just ensure it executes.
    testing::internal::CaptureStdout();
    const ExecuteResult r = executeSelect(&sel, t);
    (void)testing::internal::GetCapturedStdout();

    EXPECT_EQ(r, EXECUTE_SUCCESS);

    freeTable(t);
}

TEST(Smoke, ExecuteStatement_Dispatches) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    Statement st{};
    st.type = STATEMENT_INSERT;
    st.row_to_insert.id = 1;
    std::memset(st.row_to_insert.username, 0, USERNAME_SIZE);
    std::memset(st.row_to_insert.email, 0, EMAIL_SIZE);
    std::memcpy(st.row_to_insert.username, "bob", 3);
    std::memcpy(st.row_to_insert.email, "a@b.com", 6);

    const ExecuteResult r = executeStatement(&st, t);
    EXPECT_EQ(r, EXECUTE_SUCCESS);

    freeTable(t);
}

TEST(Smoke, DoMetaCommand_Unrecognized) {
    Table* t = newTable();
    ASSERT_NE(t, nullptr);

    InputBuffer ib = makeIB(".not_a_command");
    const MetaCommandResult r = doMetaCommand(&ib, t);

    EXPECT_EQ(r, META_COMMAND_UNRECOGNIZED_COMMAND);

    freeIB(&ib);
    freeTable(t);
}