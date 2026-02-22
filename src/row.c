#include "sqlite/row.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void serializeRow(const Row* source, void* destination)
{
    assert(source && destination);

    if (!source || !destination)
    {
        printf("Source or Destination invalid for serialization");
        return;
    }

    memcpy(destination + ID_OFFSET, &source->id, ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &source->username, USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &source->email, EMAIL_SIZE);
}

void deserializeRow(const void* source, Row* destination)
{
    assert(source && destination);

    if (!source || !destination)
    {
        printf("Source or Destination invalid for deserialization");
        return;
    }

    memcpy(&destination->id, source + ID_OFFSET, ID_SIZE);
    memcpy(&destination->username, source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&destination->email, source + EMAIL_OFFSET, EMAIL_SIZE);
}

void printRow(Row* row)
{
    if (!row)
    {
        printf("Invalid pointer to row");
        return;
    }
    printf("Id: %d, Username: %s, Email: %s\n", row->id, row->username, row->email);
}