
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "includes/tokens.h"
#include "includes/tokenizer.h"
#include "includes/variable.h"
#include "includes/parser.h"
#include "includes/simulate.h"

#define MAX_CODE_LENGTH 100000
#define MAX_STATEMENT_NUMBER 100

char* filename;
FILE* f;

StatementList statementList, syntaxCheckList;
int nStatements = 0;

VariableList varList, syntaxCheckVarList;

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
        // printf("Declaration Statement is Incorrect. Exiting\n");
        f = fopen(filename, "r");
        // return 1;
    }
    

    fread(codeStr, MAX_CODE_LENGTH, 1, f);
    int compileStatus = tokenizeStatements(codeStr, &statementList, &varList);
    // if(compileStatus != 0)
    // {
    //     printf("Compilation Failed. Exiting\n");
    //     return 1;
    // }
    printAllStatments(&statementList);

    Node* root = newNode("snippet");
    
    // parseExpression(&statementList.list[2], root, 2, statementList.list[2].len - 1);
    parseAllStatements(&statementList, root);

    printf("Printing Parse Tree\n");
    writeParseTree(root, NULL);
    printf("\n");

    printf("Output:\n");
    simulate(root, &varList);
    printAllVariables(&varList);

    
    f = fopen(filename, "r");
    fread(codeStr, MAX_CODE_LENGTH, 1, f);
    tokenizeStatementsV2(codeStr, &syntaxCheckList, &syntaxCheckVarList);
    printAllStatments(&syntaxCheckList);
    
    return 0;
}