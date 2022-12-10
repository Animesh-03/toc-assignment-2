
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tokens.h"
#include "tokenizer.h"
#include "variable.h"
#include "parser.h"

#define MAX_CODE_LENGTH 100000
#define MAX_STATEMENT_NUMBER 100
#define DEF 333333
char* filename;
FILE* f;
StatementList statementList;
int nStatements = 0;
VariableList varList;

bool equal(char* a,char* b)
{
    return !strcmp(a,b);
}

int dfs(Node* v)
{
    if(v==NULL||equal(v->name,";")||equal(v->name,"(")||equal(v->name,")"))
        return 0;
    if(equal(v->name,"snippet"))
    {
        dfs(v->next[0]);
        if(v->len>2)
            dfs(v->next[2]);
        return 0;
    }
    //printf("%s\n",v->name);
    if(equal(v->name,"assign_stmt"))
    {
        int rhs = dfs(v->next[2]);
        updateVariable(&varList,v->next[0]->next[0]->name,rhs);
    }
    else if(equal(v->name,"read_stmt"))
    {
        int t;
        scanf("%d",&t);
        updateVariable(&varList,v->next[1]->next[0]->name,t);
    }
    else if(equal(v->name,"write_stmt"))
    {
        int rhs = dfs(v->next[1]);
        printf("%d\n",rhs);
    }
    else if(equal(v->name,"for-loop"))
    {
        dfs(v->next[2]);
        while(dfs(v->next[4]))
        {
            dfs(v->next[9]);
            dfs(v->next[6]);
        }
    }   
    else
    {
        if(equal(v->name,"var"))
        {
            return getVariableVal(&varList , v->next[0]->name);
        }
        else if(equal(v->name,"const"))
        {
            return atoi(v->next[0]->name);
        }
        
        if(equal(v->name,"operand"))
        {
            if(v->len == 1)
                return dfs(v->next[0]);
            return dfs(v->next[1]);
        }

        if(v->len>1)
        {
            int lhs = dfs(v->next[0]);
            int rhs = dfs(v->next[2]);
            int ret;
            if(equal(v->next[1]->name,"+"))
            {
                ret = lhs+rhs;
            } 
            else if(equal(v->next[1]->name,"-"))
            {
                ret = lhs-rhs;
            } 
            else if(equal(v->next[1]->name,"*"))
            {
                ret = lhs*rhs;
            } 
            else if(equal(v->next[1]->name,"/"))
            {
                ret = lhs/rhs;
            } 
            else if(equal(v->next[1]->name,"=="))
            {
                ret = lhs == rhs;
            } 
            else if(equal(v->next[1]->name,">"))
            {
                ret = lhs>rhs;
            } 
            return ret;
        }


        return dfs(v->next[0]);
    }
}
























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
        f = fopen(filename, "r");
        // return 1;
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
    printf("Output:\n");
    dfs(root);

    //printAllVariables(&varList);
    return 0;
}