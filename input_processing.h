#ifndef SQLITE_INPUT_PROCESSING_H
#define SQLITE_INPUT_PROCESSING_H
#include <stdio.h>

#include "input_buffer.h"

void printPrompt();
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
void readInput(InputBuffer* input_buffer);

#endif //SQLITE_INPUT_PROCESSING_H