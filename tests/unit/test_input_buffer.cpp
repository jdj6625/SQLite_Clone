#include <gtest/gtest.h>

extern "C" {
#include "sqlite/input_buffer.h"
}

TEST(InputBuffer, Initialization)
{
   InputBuffer* buf = newInputBuffer();
   ASSERT_NE(buf, nullptr);

   ASSERT_EQ(buf->buffer, nullptr);
   ASSERT_EQ(buf->buffer_length, 0);
   ASSERT_EQ(buf->input_length, 0);
   closeInputBuffer(buf);
}

TEST(CloseBuffer, CloseAcceptsNullptr)
{
   EXPECT_NO_THROW(closeInputBuffer(nullptr));
}