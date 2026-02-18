#include "input_processing.h"
#include <stdlib.h>

void printPrompt()
{
    { printf("db > "); }
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    if (!lineptr || !n || !stream) return -1;

    if (*lineptr == nullptr)
    {
        *n = 128;
        *lineptr = malloc(*n);
        if (!*lineptr) return -1;
    }

    size_t pos = 0;
    int c;

    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            *n *= 2;
            char *newptr = realloc(*lineptr, *n);
            if (!newptr) return -1;
            *lineptr = newptr;
        }

        (*lineptr)[pos++] = (char)c;

        if (c == '\n')
            break;
    }

    if (pos == 0 && c == EOF)
        return -1;

    (*lineptr)[pos] = '\0';
    return (int)pos;
}

void readInput(InputBuffer* input_buffer)
{
    const ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read < 0)
    {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytes_read-1;
    input_buffer->buffer[bytes_read-1] = 0;
}
