#ifndef VARIABLE_H
#define VARIABLE_H

#include <string.h>

#define VAR_ALREADY_EXISTS -1
#define VAR_NOT_FOUND -2
#define VAR_UPDATE_FAILED -3


struct variable {
    char name[20];
    int value;
};

typedef struct variable Variable;

struct variableList {
    Variable vars[20];
    int len;  
};

typedef struct variableList VariableList;

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

int getVariableVal(VariableList* varList, char* varName)
{
    int index = findVariable(varList, varName);
    if(index == VAR_NOT_FOUND)
    {
        return (VAR_NOT_FOUND);
    }
    else return varList->vars[index].value;
}

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