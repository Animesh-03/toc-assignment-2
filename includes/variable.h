#ifndef VARIABLE_H
#define VARIABLE_H

#include <string.h>

#define VAR_ALREADY_EXISTS -1
#define VAR_NOT_FOUND -2
#define VAR_UPDATE_FAILED -3
#define VAR_LEN 1000


// Struct stores the value and name of the variable with a \n at the end to make it compatible with string functions
struct variable {
    char name[20];
    int value;
};

typedef struct variable Variable;

// Struct stores the all the variables defined at the beginning of the program
struct variableList {
    Variable vars[VAR_LEN];
    int len;  
};

typedef struct variableList VariableList;

// Finds a variable by name in the variable List and returns the index if found else returns VAR_NOT_FOUND
int findVariable(VariableList* varList , char* varName)
{
    for(int i = 0; i < varList->len; i++)
    {
        if(!strcmp(varList->vars[i].name, varName))
        {
            return i;
        }
    }

    return VAR_NOT_FOUND;
}

// Adds a variable with a given name into the list and returns its index.
// If the variable name already exists in the the list then returns VAR_ALREADY_EXISTS
int addVariable(VariableList* varList, char* varName)
{
    int index = findVariable(varList, varName);
    if(index >= 0)
    {
        return (VAR_ALREADY_EXISTS);
    }

    strcpy(varList->vars[varList->len].name, varName);
    varList->vars[varList->len].value = 0;
    varList->len++;

    return (varList->len - 1);
}

// Returns the variable' value given the name
// If the variable name is not found in the list then returns VAR_NOT_FOUND
int getVariableVal(VariableList* varList, char* varName)
{
    int index = findVariable(varList, varName);
    if(index == VAR_NOT_FOUND)
    {
        return (VAR_NOT_FOUND);
    }
    else return varList->vars[index].value;
}

// Returns the updated value of the variable if successful
// If the variable name is not found in the list then returns VAR_UPDATE_FAILED
int updateVariable(VariableList* varList, char* varName, int val)
{
    int index = findVariable(varList, varName);
    if(index == VAR_NOT_FOUND)
    {
        return (VAR_UPDATE_FAILED);
    }

    varList->vars[index].value = val;
    return val;
}

// Prints all the variables in the list in a human readable form
void printAllVariables(VariableList* varList)
{
    printf("\nPrinting all variables\n");
    if(varList->len == 0)
    {
        printf("No variables in memory\n");
    }

    for(int i = 0; i < varList->len; i++)
    {
        printf("%d. %s = %d\n", i+1, varList->vars[i].name, varList->vars[i].value);
    }
    printf("\n");
}


#endif