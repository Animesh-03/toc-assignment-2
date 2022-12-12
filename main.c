
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
#include "includes/syntaxChecker.h"

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

    Node* root = newNode("snippet");
    Node* program = newNode("program");
    

    if(declareStatus != 0)
    {
        // printf("Declaration Statement is Incorrect. Exiting\n");
        f = fopen(filename, "r");
        // root = newNode("nonempty-snippet");
        
        // return 1;
    }
    else
    {
        parseVariables(&varList, program);
    }
    pushChild(program, root);
    

    fread(codeStr, MAX_CODE_LENGTH, 1, f);
    int compileStatus = tokenizeStatements(codeStr, &statementList, &varList);

    // printAllStatments(&statementList);

      
    parseAllStatements(&statementList, root);

    writeParseTree(program, NULL);
    printf("\n");

    f = fopen(filename, "r");
    fread(codeStr, MAX_CODE_LENGTH, 1, f);
    tokenizeStatementsV2(codeStr, &syntaxCheckList, &syntaxCheckVarList);
    printAllStatments(&syntaxCheckList);

    if(checkSyntax(&syntaxCheckList, 0) == -1)
    {
        exit(1);
    }

    printf("Output:\n");
    simulate(root, &varList);
    printAllVariables(&varList);

    
    printf("Printing Parse Tree\n");
    
    return 0;
}

/*
for(c = a + b; c > 0; c = c - 1)
{
    a = a + ( b * c);
};
*/