#ifndef TOKENS_H
#define TOKENS_H

enum TokenType {KEYWORD, VARIABLE, CONSTANT, PARANTHESES};

typedef enum TokenType TokenType;

struct token {
    char name[20];
    TokenType type;
};

typedef struct token Token;

struct tokenList {
    int len;
    Token list[20];
    int cursor;
};

typedef struct tokenList Statement;

void addToken(Statement* statment, char* name, TokenType type)
{
    strcpy(statment->list[statment->len].name, name);
    statment->list[statment->len].type = type;
    statment->len++;
}

void nextToken(Statement* statement)
{
    statement->cursor++;
}

struct stmtList {
    Statement list[100];
    int len;
    int cursor;
};

typedef struct stmtList StatementList;

void nextStatment(StatementList* statementList)
{
    statementList->cursor++;
}

void resetStatementListCursor(StatementList* statementList)
{
    statementList->cursor = 0;
}



#endif