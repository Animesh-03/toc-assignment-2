
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "includes/tokens.h"
#include "includes/tokenizer.h"

char* filename;
FILE* f;

TokenList tokenList;
TokenList variableLookup;

int main(int argc, char** argv) {

    filename = "basicC.txt";
    f = fopen(filename, "r");

    char codeStr[100000]; 
    fread(codeStr, 100000, 1, f);

    tokenize(codeStr);


    
    return EXIT_SUCCESS;
}