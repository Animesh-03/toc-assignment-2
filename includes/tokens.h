#ifndef TOKENS_H
#define TOKENS_H

enum TokenType {KEYWORD, VARIABLE, CONSTANT, PARANTHESES};

typedef enum TokenType TokenType;

struct token {
    char name[20];
    int nameLen;
    int value;
    TokenType type;
};

typedef struct token Token;

struct tokenList {
    int size;
    Token list[100];
};

typedef struct tokenList TokenList;


#endif