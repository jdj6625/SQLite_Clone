#include "input_buffer.h"
#include <stdlib.h>

InputBuffer* newInputBuffer()
{
    InputBuffer* input_buff = malloc(sizeof(InputBuffer));
    input_buff->buffer = nullptr;
    input_buff->buffer_length = 0;
    input_buff->input_length = 0;

    return input_buff;
}

void closeInputBuffer(InputBuffer* input_buffer)
{
    free(input_buffer->buffer);
    input_buffer->buffer = nullptr;
    free(input_buffer);
    input_buffer = nullptr;
}