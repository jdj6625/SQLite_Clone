#ifndef SQLITE_INPUT_BUFFER_H
#define SQLITE_INPUT_BUFFER_H
#include <assert.h>

typedef struct
{
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
}InputBuffer;

InputBuffer* newInputBuffer();
void closeInputBuffer(InputBuffer* input_buffer);

#endif //SQLITE_INPUT_BUFFER_H