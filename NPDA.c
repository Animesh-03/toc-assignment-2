#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LEN 1000
#define MAX_RULES 100
#define RULE_LENGTH 100

typedef struct RightSide
{
	int len;
	int* symbols;
}RHS;

typedef struct rule
{
	int stack_s;
	int string_s;
	RHS rhs;
}RULE;

RULE cache[MAX_RULES];
int n;

void addRule(int stack_s,int string_s,int* rhs,int len)
{
	RULE temp;
	temp.stack_s = stack_s;
	temp.string_s = string_s;
	temp.rhs.len = len;
	temp.rhs.symbols = rhs;
	cache[n++] = temp;
}

RHS getRHS(int stack_s,int string_s)
{

}

int accepts(int i,int str[MAX_LEN])
{

}

int main()
{
	n = 0;
	int ar[5] = {1,2,3,4,5};
	addRule(0,0,ar,3);
}