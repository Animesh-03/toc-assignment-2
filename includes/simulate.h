#ifndef SIMULATE_H
#define SIMULATE_H

#include <string.h>

#include "parser.h"
#include "variable.h"

// Returns if two string are equal
bool nodeEquals(char* a,char* b)
{
    return !strcmp(a,b);
}

// Simulates the given parse tree for a given node
int simulate(Node* root, VariableList* varList)
{
    if(root==NULL||nodeEquals(root->name,";")||nodeEquals(root->name,"(")||nodeEquals(root->name,")"))
    {
        return 0;   // If root is null or a delimiter then ignore
    }
        
    if(nodeEquals(root->name,"snippet"))
    {
        simulate(root->next[0], varList);   // Simulate the first statement
        if(root->len>2)
            simulate(root->next[2], varList);   // Simulate the snippet which is a child of the current snippet
        return 0;
    }

    if(nodeEquals(root->name,"assign_stmt"))
    {
        int rhs = simulate(root->next[2], varList);                              // Get the value of the rhs in the assignment statement
        int status = updateVariable(varList,root->next[0]->next[0]->name,rhs);   // Update the variable on LHS to the value of the expression obtained above
        if(status == VAR_UPDATE_FAILED)
        {
            printf("Variable %s not found\n", root->next[0]->next[0]->name);
            exit(1);
        }
    }
    else if(nodeEquals(root->name,"read_stmt"))
    {
        int tmp;
        scanf("%d",&tmp);                                                        // Read the value of the input from console
        int status = updateVariable(varList,root->next[1]->next[0]->name,tmp);   // Update the variable on the RHS to the value
        if(status == VAR_UPDATE_FAILED)
        {
            printf("Variable %s not found\n", root->next[1]->next[0]->name);
            exit(1);
        }
    }
    else if(nodeEquals(root->name,"write_stmt"))
    {
        int rhs = simulate(root->next[1], varList);                 // Get the value on RHS of the write statement
        printf("%d\n",rhs);                                         // Print the value obtained above
    }
    else if(nodeEquals(root->name,"for-loop"))
    {
        simulate(root->next[2], varList);                           // Run the first assignment statement
        while(simulate(root->next[4], varList))                     // Check the 2nd expression in the for loop
        {
            simulate(root->next[9], varList);                       // Run the code inside the {} block of the for loop
            simulate(root->next[6], varList);                       // Run the 3rd assignment statement in the for loop
        }
    }   
    else
    {
        if(nodeEquals(root->name,"var"))
        {
            int val = getVariableVal(varList , root->next[0]->name);
            if(val == VAR_NOT_FOUND)
            {
                printf("Variable %s not found\n", root->next[0]->name);
                exit(1);
            }
            return val;   // Returns the variable value stored in the token
        }
        else if(nodeEquals(root->name,"const"))
        {
            return atoi(root->next[0]->name);                       // Returns the constant value stored in the token
        }
        
        if(nodeEquals(root->name,"operand"))
        {
            if(root->len == 1)                                      // If operand cascades down to a variable or constant the return their value
                return simulate(root->next[0], varList);            // Else the operand creates a new bracketed expression so simulate that expression
            return simulate(root->next[1], varList);
        }

        if(root->len>1)                                             // If the expression has more than 1 child then it is an arithmetic expression 
        {
            int lhs = simulate(root->next[0], varList);             // Get tge value of the lhs child
            int rhs = simulate(root->next[2], varList);             // Get the value of the rhs child
            int ret;                                                // Stores the value of the expression depending on the operator used
            if(nodeEquals(root->next[1]->name,"+"))
            {
                ret = lhs+rhs;
            } 
            else if(nodeEquals(root->next[1]->name,"-"))
            {
                ret = lhs-rhs;
            } 
            else if(nodeEquals(root->next[1]->name,"*"))
            {
                ret = lhs*rhs;
            } 
            else if(nodeEquals(root->next[1]->name,"/"))
            {
                ret = lhs/rhs;
            } 
            else if(nodeEquals(root->next[1]->name,"=="))
            {
                ret = lhs == rhs;
            } 
            else if(nodeEquals(root->next[1]->name,">"))
            {
                ret = lhs>rhs;
            } 
            return ret;
        }

        return simulate(root->next[0], varList);            // Go to the next child
    }
}


#endif