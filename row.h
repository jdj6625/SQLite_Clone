#ifndef SQLITE_ROW_H
#define SQLITE_ROW_H
#include <stdint.h>
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

constexpr size_t COLUMN_USERNAME_SIZE = 32;
constexpr size_t COLUMN_EMAIL_SIZE = 255;

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}Row;

constexpr uint32_t ID_SIZE = size_of_attribute(Row, id);
constexpr uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
constexpr uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
constexpr uint32_t ID_OFFSET = 0;
constexpr uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
constexpr uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void serializeRow(const Row* source, void* destination);
void deserializeRow(const void* source, Row* destination);
void printRow(Row* row);

#endif //SQLITE_ROW_H