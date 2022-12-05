
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "includes/tokens.h"
#include "includes/tokenizer.h"

#define MAX_CODE_LENGTH 100000

char* filename;
FILE* f;

TokenList tokenList;
TokenList variableLookup;

int main(int argc, char** argv) {

    filename = argv[1];
    f = fopen(filename, "r");
    if(f == NULL)
    {
        perror("Input file not found");
        exit(1);
    }

    char codeStr[MAX_CODE_LENGTH]; 
    fread(codeStr, MAX_CODE_LENGTH, 1, f);

    tokenize(codeStr);


    
    return 0;
}