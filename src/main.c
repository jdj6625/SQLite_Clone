#include <stdio.h>

#include "sqlite/input_buffer.h"
#include "sqlite/input_processing.h"

int main(int argc, char** argv)
{
    Table* table = newTable();
    InputBuffer* input_buff = newInputBuffer();
    while (true)
    {
        printPrompt();
        readInput(input_buff);

        if (input_buff->buffer[0] == '.')
        {
            switch (doMetaCommand(input_buff, table))
            {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buff->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepareStatement(input_buff, &statement))
        {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax Error. Could not parse statement.\n");
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buff->buffer);
                continue;
        }

        switch (executeStatement(&statement, table))
        {
            case EXECUTE_SUCCESS:
                printf("Executed.\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error. Table full.\n");
                break;
            case EXECUTE_FAILURE:
                printf("Execution failed.\n");
                break;
        }
    }
}
