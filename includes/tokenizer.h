#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

// Parsing the input STRING.
void tokenize(char* str)
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
			

        // If the current char is an operator( a delimiter whose length is 1) then classify it as an operator
		if(isDelimiter(str[right]) == true && left == right) 
        {
            if(str[right] == '=' && str[right+1] == '=')
            {
                right++;
                printf("'%s' is a logical equals operator\n", subString(str, left, right));
                right++;
                left = right;
            }
			else if (isOperator(str[right]) == true)
				printf("'%c' IS AN OPERATOR\n", str[right]);

			right++;
			left = right;
		}
        // If current string is a delimiter and length of current string i.e, str[left:right-1] > 1 then it must be one of variable, integer or keyword
        else if(isDelimiter(str[right]) == true && left != right || (right == len && left != right)) 
        {
            // Get the current sub-string i.e string[left:right-1]
			char* subStr = subString(str, left, right - 1);

			if (isKeyword(subStr) == true)
				printf("'%s' IS A KEYWORD\n", subStr);

			else if (isInteger(subStr) == true)
				printf("'%s' IS AN INTEGER\n", subStr);

			else if (validIdentifier(subStr) == true
					&& isDelimiter(str[right - 1]) == false)
				printf("'%s' IS A VALID IDENTIFIER\n", subStr);

			else if (validIdentifier(subStr) == false
					&& isDelimiter(str[right - 1]) == false)
				printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);

			left = right;
		}
	}
	return;
}

#endif