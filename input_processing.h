#ifndef SQLITE_INPUT_PROCESSING_H
#define SQLITE_INPUT_PROCESSING_H
#include <stdint.h>
#include <stdio.h>
#include "input_buffer.h"
#include "row.h"
#include "table.h"


typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR
}PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_FAILURE,
    EXECUTE_TABLE_FULL
}ExecuteResult;

void printPrompt();
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
void readInput(InputBuffer* input_buffer);
MetaCommandResult doMetaCommand(InputBuffer* input_buffer, Table* table);
PrepareResult prepareStatement(const InputBuffer* input_buffer, Statement* statement);
ExecuteResult executeInsert(const Statement* statement, Table* table);
ExecuteResult executeSelect(const Statement* statement, Table* table);
ExecuteResult executeStatement(const Statement* statement, Table* table);

#endif //SQLITE_INPUT_PROCESSING_H