#ifndef TOKENS_H
#define TOKENS_H

enum TokenType {KEYWORD, VARIABLE, CONSTANT, PARANTHESES, OPERATOR};

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

void addEmptyStatement(StatementList* statementList)
{
    statementList->len++;
}

void addTokenToLastStatement(StatementList* statementList, char* name, TokenType type)
{
    addToken(&statementList->list[statementList->len], name, type);
}

void nextStatment(StatementList* statementList)
{
    statementList->cursor++;
}

void resetStatementListCursor(StatementList* statementList)
{
    statementList->cursor = 0;
}

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