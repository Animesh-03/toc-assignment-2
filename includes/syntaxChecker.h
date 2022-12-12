#ifndef SYNTAX_CHECKER_H
#define SYNTAX_CHECKER_H

#include "tokens.h"

int checkForLoopSyntax(StatementList* syntaxCheckList, int* statementCursor);
int checkSyntaxForStatement(StatementList* syntaxCheckList , Statement* statement, int *statementCursor);

int checkDeclareStatementSyntax(Statement* declareStatement)
{
    int isLastTokenVar = 0;
    for(int i = 1; i < declareStatement->len - 1; i++)
    {
        if(isLastTokenVar)
        {
            if(declareStatement->list[i].type != COMA)
            {
                return -1;
            }
            isLastTokenVar = 0;
        }
        else
        {
            if(declareStatement->list[i].type != VARIABLE)
            {
                return -1;
            }
            isLastTokenVar = 1;
        }
    }
    if(isLastTokenVar == 0 || declareStatement->list[declareStatement->len - 1].type != SEMI_COLON)
    {
        return -1;
    }
    return 0;
}

int checkExpressionSyntax(Statement* statement, int start, int ignoreSemiColon)
{
    if((statement->len - start)%2 == 1)
    {
        return -1;
    }

    int bracketDepth = 0, isLastTokenVar = 0;
    for(int i = start; i < statement->len - 1; i++)
    {
        if(tokenEquals(&statement->list[i], "("))
        {
            bracketDepth++;
        }
        else if(tokenEquals(&statement->list[i], ")"))
        {
            bracketDepth--;
            if(bracketDepth < 0)
            {
                return -1;
            }
        }
        else if(isLastTokenVar == 1)
        {
            if(statement->list[i].type != OPERATOR)
            {
                return -1;
            }
            isLastTokenVar = 0;
        }
        else
        {
            if(statement->list[i].type != VARIABLE && statement->list[i].type != CONSTANT)
            {
                return -1;
            }
            isLastTokenVar = 1;
        }
    }

    if(ignoreSemiColon == 1)
        return 0;

    if(bracketDepth != 0 || ( statement->list[statement->len - 1].type != SEMI_COLON))
    {
        return -1;
    }

    return 0;
}

int checkAssignmentStatementSyntax(Statement* statement, int ignoreSemiColon)
{
    if(statement->len < 4)
    {
        return -1;
    }
    if(statement->list[0].type != VARIABLE && !tokenEquals(&statement->list[1], "="))
    {
        return -1;
    }
    if(checkExpressionSyntax(statement, 2, ignoreSemiColon) == -1)
    {
        return -1;
    }

    return 0;
}

int checkIOStatementSyntax(Statement* statement)
{
    if(statement->len != 3)
    {
        return -1;
    }

    if((tokenEquals(&statement->list[0], "read") || tokenEquals(&statement->list[0], "write")) && (statement->list[1].type == VARIABLE || statement->list[1].type == CONSTANT) && tokenEquals(&statement->list[2], ";"))
    {
        return 0;
    }
    return -1;
}

int checkForLoopSyntax(StatementList* syntaxCheckList, int* statementCursor)
{
    Statement* forStatement = &syntaxCheckList->list[*statementCursor];
    if(forStatement->len != 2)
    {
        return -1;
    }
    if(!tokenEquals(&forStatement->list[0], "for") || !tokenEquals(&forStatement->list[1], "("))
    {
        return -1;
    }

    if( checkAssignmentStatementSyntax(&syntaxCheckList->list[*statementCursor + 1], 0) == -1 || 
        checkExpressionSyntax(&syntaxCheckList->list[*statementCursor + 2], 0, 0) == -1 ||
        checkAssignmentStatementSyntax(&syntaxCheckList->list[*statementCursor + 3],1) == -1
    )
    {
        return -1;
    }

    // if(syntaxCheckList->list[*statementCursor + 4].len != 1 || !tokenEquals(&syntaxCheckList->list[*statementCursor + 4].list[0], ")"))
    // {
    //     return -1;
    // }

    *statementCursor += 5;
    Statement* statement = &syntaxCheckList->list[*statementCursor];

    while(!tokenEquals(&statement->list[0], "}"))
    {
        if(checkSyntaxForStatement(syntaxCheckList, statement, statementCursor) == -1)
        {
            return -1;
        }

        *statementCursor = *statementCursor + 1;
        if(*statementCursor >= syntaxCheckList->len)
        {
            return -1;
        }
        statement = &syntaxCheckList->list[*statementCursor];
    }
    if(!tokenEquals(&statement->list[0], "}") || !tokenEquals(&statement->list[1], ";"))
    {
        return -1;
    }

    return 0;
}

int checkSyntaxForStatement(StatementList* syntaxCheckList , Statement* statement, int *statementCursor)
{
    if(statement->type == ASSIGNMENT)
        {
            if(checkAssignmentStatementSyntax(statement, 0) == -1)
            {
                printf("Statement #%d invalid\n", *statementCursor + 1);
                return -1;
            }
        }
        else if(statement->type == IO)
        {
            if(checkIOStatementSyntax(statement) == -1)
            {
                printf("Statement #%d invalid\n", *statementCursor + 1);
                return -1;
            }
        }
        else if(statement->type == FOR)
        {
            checkForLoopSyntax(syntaxCheckList, statementCursor);
        }
}

int checkSyntax(StatementList* syntaxCheckList, int statementCursor)
{

    for(int j = 0; j < syntaxCheckList->len; j++)
    {
        if(tokenEquals(&syntaxCheckList->list[j].list[0], ")"))
        {
            printf("Statement #%d invalid\n", j + 1);
            return -1;
        }
    }

    if(tokenEquals(&syntaxCheckList->list[0].list[0], "int"))
    {
        if(checkDeclareStatementSyntax(&syntaxCheckList->list[0]) == -1)
        {
            printf("Declare Statement Invalid\n");
            return -1;
        }
        statementCursor++;
    }

    while(statementCursor < syntaxCheckList->len)
    {
        Statement* statement = &syntaxCheckList->list[statementCursor];

        if(checkSyntaxForStatement(syntaxCheckList, statement, &statementCursor))
        {
            printf("Statement #%d invalid\n", statementCursor + 1);
            return -1;
        }

        statementCursor++;
    }


    printf("Syntax Check Complete\n");
    return 0;
}



#endif