#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "variable.h"
#include "tokens.h"

// Returns 'true' if the character is an OPERATOR.
bool isOperator(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '>' || ch == '=')
		return (true);
	return (false);
}

// Returns 'true' if the character is a DELIMITER.
bool isDelimiter(char ch)
{
	if (ch == ' ' || ch == ',' || ch == ';' || ch == '>' ||
		ch == '=' || ch == '(' || ch == ')' ||
		ch == '{' || ch == '}' || ch == '\n' || isOperator(ch))
		return (true);
	return (false);
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(char* str)
{
	for(int i = 0; i < strlen(str); i++)
    {
        if((str[i] < 'a' || str[i] > 'z') || isDelimiter(str[i]))
            return false;
    }
	return (true);
}

// Returns 'true' if the string is a KEYWORD.
bool isKeyword(char* str)
{
	if (!strcmp(str, "int") || !strcmp(str, "read") || !strcmp(str, "write") || !strcmp(str, "for"))
		return (true);
	return (false);
}

// Returns 'true' if the string is an INTEGER.
bool isInteger(char* str)
{
	int i, len = strlen(str);

	if (len == 0)
		return (false);
	for (i = 0; i < len; i++) {
		if ((str[i] < '0' || str[i] > '9') || (str[i] == '-' && i > 0))
			return (false);
	}
	return (true);
}

// Extracts the SUBSTRING.
char* subString(char* str, int left, int right)
{
	int i;
	char* subStr = (char*) malloc(sizeof(char) * (right - left + 2));

	for (i = left; i <= right; i++)
		subStr[i - left] = str[i];
	subStr[right - left + 1] = '\0';
	return (subStr);
}

int tokenizeVariables(char* str, VariableList* varList)
{
	int left = 0, right = 0;
	int len = strlen(str);
	int foundIntKeyword = 0;

    // Run till entire code is read
	while(right <= len && left <= right) {

        // If the current char is not a delimiter then increase the range of string to the right
		if(isDelimiter(str[right]) == false)
        {
            right++;
        }
		else
		{
			right++;
			left = right;
		}
			
        // If current string is a delimiter and length of current string i.e, str[left:right-1] > 1 then it must be one of variable, integer or keyword
		if(isDelimiter(str[right]) == true && left != right || (right == len && left != right)) 
        {
            // Get the current sub-string i.e string[left:right-1]
			char* subStr = subString(str, left, right - 1);

			if (isKeyword(subStr) == true && !strcmp(subStr, "int"))
			{
				// printf("'%s' Keyword found\n", subStr);
				foundIntKeyword = 1;
			}
			else if (validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false)
			{
				if(!foundIntKeyword)
				{
					printf("Expected 'int' before identifier\n");
					return -1;
				}

				// printf("'%s' IS A VALID IDENTIFIER\n", subStr);
				int status = addVariable(varList, subStr);
				if(status == VAR_ALREADY_EXISTS)
				{
					printf("'%s' VAR_ALREADY_EXISTS\n", subStr);
					return -1;
				}
			}
			else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false)
			{
				printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);
				return -1;
			}	

			left = right;
		}
	}
	return 0;
}

// Parsing the input STRING.
int tokenizeStatements(char* str, StatementList* statementList)
{
	int left = 0, right = 0;
	int len = strlen(str);

    // Run till entire code is read
	while(right <= len && left <= right) {

        // If the current char is not a delimiter then increase the range of string to the right
		if(isDelimiter(str[right]) == false)
        {
            right++;
        }
			

        // If the current char is an operator (which is also a delimiter) then classify it as an operator
		if(isDelimiter(str[right]) == true && left == right) 
        {
            if(str[right] == '=' && str[right+1] == '=')
            {
                right++;
				char* subsStr = subString(str, left, right);
                // printf("'%s' is a logical equals operator\n", subsStr);
				addTokenToLastStatement(statementList, subsStr, OPERATOR);
                right++;
                left = right;
            }
			else if (isOperator(str[right]) == true)
			{
				// printf("'%c' IS AN OPERATOR\n", str[right]);
				char* subStr = subString(str, right, right);
				addTokenToLastStatement(statementList, subStr, OPERATOR);
			}
				

			right++;
			left = right;
		}
        // If current string is a delimiter and length of current string i.e, str[left:right-1] > 1 then it must be one of variable, integer or keyword
        else if(isDelimiter(str[right]) == true && left != right || (right == len && left != right)) 
        {
            // Get the current sub-string i.e string[left:right-1]
			char* subStr = subString(str, left, right - 1);

			if (isKeyword(subStr) == true)
			{
				// printf("'%s' IS A KEYWORD\n", subStr);
				addTokenToLastStatement(statementList, subStr, KEYWORD);
			}
			else if (isInteger(subStr) == true)
			{
				// printf("'%s' IS AN INTEGER\n", subStr);
				addTokenToLastStatement(statementList, subStr, CONSTANT);
			}
			else if (validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false)
			{
				// printf("'%s' IS A VALID IDENTIFIER\n", subStr);
				addTokenToLastStatement(statementList, subStr, VARIABLE);
			}
			else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false)
			{
				// printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);
				return -1;
			}

			left = right;
			if(str[right] == ';')
			{
				addEmptyStatement(statementList);
			}
		}
	}
	return 0;
}

#endif