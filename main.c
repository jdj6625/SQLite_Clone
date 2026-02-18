#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_buffer.h"
#include "input_processing.h"

int main(int argc, char** argv)
{
    InputBuffer* input_buff = newInputBuffer();
    while (true)
    {
        printPrompt();
        readInput(input_buff);

        if (strncmp(input_buff->buffer, ".exit", 26) == 0)
        {
            closeInputBuffer(input_buff);
            exit(EXIT_SUCCESS);
        }

        else
        {
            printf("Invalid Command %s\n", input_buff->buffer);
        }
    }
}
