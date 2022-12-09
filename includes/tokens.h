#ifndef TOKENS_H
#define TOKENS_H

#include <string.h>

// The types a token can be
enum TokenType {KEYWORD, VARIABLE, CONSTANT, PARANTHESES, CURLY_BRACKETS, OPERATOR};

typedef enum TokenType TokenType;

// Struct for a single token in a statment
// Stores just the name and the type
// If the type is a variable then the lookup must be done manually by the name
struct token {
    char name[20];
    TokenType type;
};

typedef struct token Token;

// Struct for storing all the tokens in a single statement i.e, the part between two ;
struct tokenList {
    int len;
    Token list[20];
    int cursor;
};

typedef struct tokenList Statement;

// Adds token to a statment
void addToken(Statement* statment, char* name, TokenType type)
{
    strcpy(statment->list[statment->len].name, name);
    statment->list[statment->len].type = type;
    statment->len++;
}

// Moves the cursor on the statment to the next token in its list
void nextToken(Statement* statement)
{
    statement->cursor++;
}

// Struct which stores every statment in the code
struct stmtList {
    Statement list[100];
    int len;
    int cursor;
};

typedef struct stmtList StatementList;

// Adds a new token empty token by increasing the length of the statment list
void addEmptyStatement(StatementList* statementList)
{
    statementList->len++;
}

// Adds a new token to the last statment given by statmentList->list[statementList->len]
void addTokenToLastStatement(StatementList* statementList, char* name, TokenType type)
{
    addToken(&statementList->list[statementList->len], name, type);
}

// Moves the Cursor in the statment list to point to the next statement
void nextStatment(StatementList* statementList)
{
    statementList->cursor++;
}

// Sets the cursor of statmentList back to 0
void resetStatementListCursor(StatementList* statementList)
{
    statementList->cursor = 0;
}

// Prints all the statements in the statmentList in a human readable form
void printAllStatments(StatementList* statementList)
{
    printf("\nPrinting all tokens in code\n");
    printf("Number of statments: %d\n", statementList->len);
    for(int i = 0; i < statementList->len; i++)
    {
        printf("%d. [ ", i+1);
        for(int j = 0; j < statementList->list[i].len - 1; j++)
        {
            printf("'%s', ", statementList->list[i].list[j].name);
        }
        printf("'%s' ", statementList->list[i].list[statementList->list[i].len - 1].name);
        printf("]\n");
    }
}



#endif