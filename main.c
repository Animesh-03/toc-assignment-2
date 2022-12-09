
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "includes/tokens.h"
#include "includes/tokenizer.h"
#include "includes/variable.h"
#include "includes/parser.h"

#define MAX_CODE_LENGTH 100000
#define MAX_STATEMENT_NUMBER 100

char* filename;
FILE* f;

StatementList statementList;
int nStatements = 0;

VariableList varList;

int main(int argc, char** argv) {

    filename = argv[1];
    filename = "basicC.txt";
    f = fopen(filename, "r");
    if(f == NULL)
    {
        perror("Input file not found");
        exit(1);
    }

    char codeStr[MAX_CODE_LENGTH]; 
    char declareStatement[100];

    fgets(declareStatement, 100, f);
    int declareStatus = tokenizeVariables(declareStatement, &varList);
    if(declareStatus != 0)
    {
        printf("Declaration Statement is Incorrect. Exiting\n");
        return 1;
    }
    printAllVariables(&varList);

    fread(codeStr, MAX_CODE_LENGTH, 1, f);
    int compileStatus = tokenizeStatements(codeStr, &statementList, &varList);
    if(compileStatus != 0)
    {
        printf("Compilation Failed. Exiting\n");
        return 1;
    }
    printAllStatments(&statementList);

    Node* root = newNode("snippet");
    
    // parseExpression(&statementList.list[2], root, 2, statementList.list[2].len - 1);
    parseAllStatements(&statementList, root);

    printf("Printing Parse Tree\n");
    writeParseTree(root, NULL);
    printf("\n");
    
    return 0;
}