#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "tokens.h"

#define MAX_CHILD 30
FILE* ptr;
enum precedence {PR_LOGICAL, PR_SUM, PR_MULTIPLICATION, PR_VARCONST};
typedef enum precedence Precedence;

// Each node has an array of children
struct node
{
    int len;
    char* name;
    struct node* next[MAX_CHILD];
};

typedef struct node Node;

Node* newNode(char* name)
{
    Node *node = (Node*) malloc(sizeof(Node));
    node->name = name;
    return node;
}

// Pushes a new child into the children array
void pushChild(Node* root, Node* child)
{
    root->len++;
    root->next[root->len - 1] = child;
    return;
}

// Returns the precedence of a given token
// ==, > ------> 1
// +, - -------> 2
// *, / -------> 3
// Everything else ------> 5
int getPrecedence(const Token *token)
{
    if(token->type != OPERATOR)
        return PR_VARCONST;

    if(!strcmp(token->name, "==") || !strcmp(token->name, ">"))
        return PR_LOGICAL;
    else if(!strcmp(token->name, "+") || !strcmp(token->name, "-"))
        return PR_SUM;
    else if(!strcmp(token->name, "*") || !strcmp(token->name, "/"))
        return PR_MULTIPLICATION;
}

Node* handleCascade(Node* root, Precedence precedence)
{
    if(!strcmp(root->name, "expr"))
    {
        if(precedence == PR_LOGICAL)
        {
            return root;
        } 
        else if(precedence == PR_SUM)
        {
            Node* termNode = newNode("term");

            pushChild(root, termNode);

            return termNode;
        }
        else if(precedence == PR_MULTIPLICATION)
        {
            Node* termNode = newNode("term");
            Node* factorNode = newNode("factor");

            pushChild(root, termNode);
            pushChild(termNode, factorNode);

            return factorNode;
        }
        else if(precedence == PR_VARCONST)
        {
            Node* termNode = newNode("term");
            Node* factorNode = newNode("factor");
            Node* operandNode = newNode("operand");

            pushChild(root, termNode);
            pushChild(termNode, factorNode);
            pushChild(factorNode, operandNode);

            return operandNode;
        }
    }
    else if(!strcmp(root->name, "term"))
    {
        if(precedence == PR_SUM)
        {
            return root;
        }
        else if(precedence == PR_MULTIPLICATION)
        {
            Node* factorNode = newNode("factor");

            pushChild(root, factorNode);

            return factorNode;
        }
        else if(precedence == PR_VARCONST)
        {
            Node* factorNode = newNode("factor");
            Node* operandNode = newNode("operand");

            pushChild(root, factorNode);
            pushChild(factorNode, operandNode);

            return operandNode;
        }
    }
    else if(!strcmp(root->name, "factor"))
    {
        if(precedence == PR_MULTIPLICATION)
        {
            return root;
        }
        else if(precedence == PR_VARCONST)
        {
            Node* operandNode = newNode("operand");

            pushChild(root, operandNode);

            return operandNode;
        }
    }
    else if(!strcmp(root->name, "operand"))
    {
        return root;
    }
}

void parseExpression(Statement *statement, Node* root, int left, int right)
{
    if(left > right)
    {
        return;
    }
       
    // Recursively parse the tokens in between the parentheses
    if(tokenEquals(getTokenAt(statement, left), "(") && tokenEquals(getTokenAt(statement, right), ")"))
    {
        Node* paranthesesNode = handleCascade(root, PR_VARCONST);             /*          operand         */
        Node* leftParantheses = newNode("(");                                 /*          /   |   \       */ 
        Node* expression = newNode("expr");                                   /*         /    |    \      */
        Node* rightParantheses = newNode(")");                                /*        (    expr   )     */

        if(!strcmp(root->name, "operand")) {
            paranthesesNode = root;
        }

        pushChild(paranthesesNode, leftParantheses);
        pushChild(paranthesesNode, expression);
        pushChild(paranthesesNode, rightParantheses);

        parseExpression(statement, expression, left + 1, right - 1);
        return;
    }

    int lowestPrecedence = INT_MAX;
    int lowestPrecedenceIndex = -1;
    int bracketDepth = 0;

    // Search for the token with least precedence
    for(int i = left; i <= right; i++)
    {
        if(tokenEquals(getTokenAt(statement, i), "("))
        {
            bracketDepth++;
        }
        else if(tokenEquals(getTokenAt(statement, i), ")"))
        {
            bracketDepth--;
            continue;
        }

        if(bracketDepth == 0 && getPrecedence(getTokenAt(statement, i)) <= lowestPrecedence)
        {
            lowestPrecedence = getPrecedence(getTokenAt(statement, i));
            lowestPrecedenceIndex = i;
        }
    }
    if(lowestPrecedenceIndex == -1)
    {
        printf("Valid Token Not found\n");
        return;
    }

    Token* lowestPrecedenceToken = getTokenAt(statement, lowestPrecedenceIndex);
    // Create linear tree to reach the required node in the grammar
    Node* newRoot = handleCascade(root, (Precedence)lowestPrecedence);

    // Depending on the token obtained create the parse tree by adding appropriate children
    if(getPrecedence(lowestPrecedenceToken) == PR_LOGICAL)
    {
        Node* leftExpression = newNode("expr");                             /*                   expr              */
        Node* logicalOperator = newNode(lowestPrecedenceToken->name);       /*                  / | \              */
        Node* rightTerm = newNode("term");                                  /*                /   |   \            */
                                                                            /*             expr  >|==  term        */
        pushChild(newRoot, leftExpression);
        pushChild(newRoot, logicalOperator);
        pushChild(newRoot, rightTerm);

        parseExpression(statement, leftExpression, left, lowestPrecedenceIndex - 1);
        parseExpression(statement, rightTerm, lowestPrecedenceIndex + 1, right);
    }
    else if(getPrecedence(lowestPrecedenceToken) == PR_SUM)
    {
        Node* leftTerm = newNode("term");                                   /*                   term              */
        Node* logicalOperator = newNode(lowestPrecedenceToken->name);       /*                  / | \              */
        Node* rightFactor = newNode("factor");                              /*                /   |   \            */
                                                                            /*             term  +|-  factor        */
        pushChild(newRoot, leftTerm);
        pushChild(newRoot, logicalOperator);
        pushChild(newRoot, rightFactor);

        parseExpression(statement, leftTerm, left, lowestPrecedenceIndex - 1);
        parseExpression(statement, rightFactor, lowestPrecedenceIndex + 1, right);
    }
    else if(getPrecedence(lowestPrecedenceToken) == PR_MULTIPLICATION)
    {
        Node* leftFactor = newNode("factor");                               /*                 factor              */
        Node* logicalOperator = newNode(lowestPrecedenceToken->name);       /*                  / | \              */
        Node* rightOperand = newNode("operand");                            /*                /   |   \            */
                                                                            /*           factor  *|/  operand      */
        pushChild(newRoot, leftFactor);
        pushChild(newRoot, logicalOperator);
        pushChild(newRoot, rightOperand);

        parseExpression(statement, leftFactor, left, lowestPrecedenceIndex - 1);
        parseExpression(statement, rightOperand, lowestPrecedenceIndex + 1, right);
    }
    else if(getPrecedence(lowestPrecedenceToken) == PR_VARCONST)
    {
        char *nodeType = (lowestPrecedenceToken->type == VARIABLE) ? strdup("var") : strdup("const");

        Node* terminalVar = newNode(nodeType);                      /*            operand           */
        Node* terminal = newNode(lowestPrecedenceToken->name);      /*               |              */
                                                                    /*            var/const         */
        pushChild(newRoot, terminalVar);                            /*               |              */
        pushChild(terminalVar, terminal);                           /*            terminal          */
    }
}

void printRecursiveParseTree(Node* root, Node* parent)
{
    fprintf(ptr, "[%s", root->name);
    for(int i = 0;i<root->len;i++)
    {
        printRecursiveParseTree(root->next[i],root);
    }
    fprintf(ptr, "]");
}

void writeParseTree(Node* root, Node* parent) {
    ptr = fopen("parse.txt", "w");
    printRecursiveParseTree(root, parent);
    fclose(ptr);
}

void parseAllStatements(StatementList* statementList, Node* root)
{
    for(int i = 0; i < statementList->len; i++)
    {
        Statement *statement = &statementList->list[i];

        if(statement->type == ASSIGNMENT)
        {
            Node* assignmentNode = newNode("assign_stmt");
            Node* leftVar = newNode(statement->list[0].name);
            Node* equalsNode = newNode("=");
            Node* expressionNode = newNode("expr");

            pushChild(root, assignmentNode);
            pushChild(assignmentNode, leftVar);
            pushChild(assignmentNode, equalsNode);
            pushChild(assignmentNode, expressionNode);

            parseExpression(statement, expressionNode, 2, statement->len - 1);
        }
        else if(statement->type == FOR)
        {
        
        }
        else if(statement->type == IO)
        {
            Node* ioNode = newNode((!strcmp(statement->list[0].name,"read")) ? strdup("read_stmt") : strdup("write_stmt"));
            Node* ioTypeNode = newNode(statement->list[0].name);
            Node* terminalNode = newNode(statement->list[1].name);

            pushChild(root, ioNode);
            pushChild(ioNode, ioTypeNode);
            pushChild(ioTypeNode, terminalNode);
        }

    }
}


#endif


// [1[2][3]]