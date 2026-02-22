#include <gtest/gtest.h>

extern "C" {
#include "sqlite/row.h"
}

static constexpr uint8_t kSentinel = 0xCD;

TEST(Row, Serialize)
{
   Row r {};
   r.id = 1234;

   constexpr char username[COLUMN_USERNAME_SIZE] = "bob";
   constexpr char email[COLUMN_EMAIL_SIZE] = "a@b.com";

   std::memset(r.username, 0, USERNAME_SIZE);
   std::memset(r.email, 0, EMAIL_SIZE);
   std::memcpy(r.username, username, std::strlen(username));
   std::memcpy(r.email, email , std::strlen(email));

   std::vector<uint8_t> buf(ROW_SIZE, kSentinel);

   serializeRow(&r, buf.data());

   EXPECT_EQ(0, std::memcmp(buf.data() + ID_OFFSET, &r.id, ID_SIZE));

   EXPECT_EQ(0, std::memcmp(buf.data() + USERNAME_OFFSET, r.username, USERNAME_SIZE));

   EXPECT_EQ(0, std::memcmp(buf.data() + EMAIL_OFFSET, r.email, EMAIL_SIZE));
}

TEST(Row, Deserialization)
{
   constexpr uint32_t id = 1234;
   constexpr char username[COLUMN_USERNAME_SIZE] = "bob";
   constexpr char email[COLUMN_EMAIL_SIZE] = "a@b.com";
   std::vector<uint8_t> buf(ROW_SIZE, kSentinel);
   std::memcpy(buf.data()+ID_OFFSET, &id, ID_SIZE);
   std::memset(buf.data() + USERNAME_OFFSET, 0, USERNAME_SIZE);
   std::memset(buf.data() + EMAIL_OFFSET, 0, EMAIL_SIZE);
   std::memcpy(buf.data() + USERNAME_OFFSET, username, std::strlen(username));
   std::memcpy(buf.data() + EMAIL_OFFSET, email, std::strlen(email));

   Row r {};

   deserializeRow(buf.data(), &r);

   EXPECT_EQ(0, std::memcmp(buf.data() + ID_OFFSET, &r.id, ID_SIZE));

   EXPECT_EQ(0, std::memcmp(buf.data() + USERNAME_OFFSET, r.username, USERNAME_SIZE));

   EXPECT_EQ(0, std::memcmp(buf.data() + EMAIL_OFFSET, r.email, EMAIL_SIZE));
}
