#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>


#define MAX_CHILD 10

int prec[512];

void setPrec(char* c,int val)
{
    if(!strcmp(c,"=="))
        prec[0] = val;
    prec[c[0]] = val;
}

int getPrec(char* c)
{
    if(!strcmp(c,"=="))
        return prec[0];
    if(prec[c[0]] == 0)
        return INT_MAX-5;
    return prec[c[0]];
}



char* str[]= {"a","+","(","b","*","c",")"};

typedef struct node
{
    int len;
    char* val;
    struct node* next[MAX_CHILD];    
}Node;

void dfs(Node* v,Node* par)
{
    printf("%ld : %ld : %s\n",&v,&par,v->val);
    for(int i = 0;i<v->len;i++)
    {
        dfs(v->next[i],v);
    }
}

Node* getNode()
{
    Node* t = (Node*)malloc(sizeof(Node));
    return t;
}
void push(Node* n,Node* v)
{
    n->next[n->len] = v;
    n->len++;
}

void    parse(Node* v,int l,int r)
{
    if(l>r)
        return;
    if(!strcmp("(",str[l]) && !strcmp(")",str[r]))
    {
        Node* temp = getNode();
        temp->val = "operand";
        Node* lef = getNode(),*rig = getNode();
        lef->val = "(",rig->val = ")";
        Node* nex = getNode();
        nex->val = "expr";
        push(temp,lef);

        parse(nex,l+1,r-1);
        push(temp,nex);
        push(temp,rig);
        push(v,temp);
        return;
    }
    int hp = INT_MAX;
    int ind = -1;
    int depth = 0;
    for(int i = l;i<=r;i++)
    {
        if(!strcmp("(",str[i]))
            depth++;
        else if(!strcmp(")",str[i]))
        {
            depth--;
            continue;
        }
        if(depth == 0 && getPrec(str[i])<=hp)
        {
            hp = getPrec(str[i]);
            ind = i;
        }
    }
    if(ind == -1)
    {
        printf("Error!!!\n");
        return;
    }

    //printf("%d : %s\n",ind,str[ind]);
    if(getPrec(str[ind]) == INT_MAX-5)
    {

        Node* tem = getNode();
        tem->val = "VarOrCons";
        Node* tem1 = getNode();
        tem1->val = str[ind];
        push(tem,tem1);
        push(v,tem);
        return;
    }
    if(getPrec(str[ind]) == 1)
    {
        Node* left = getNode();
        left->val = "expr";
        parse(left,l,ind-1);
        push(v,left);
        Node* op = getNode();
        op->val = str[ind];
        push(v,op);
        Node* rig = getNode();
        rig->val = "term";
        parse(rig,ind+1,r);
        push(v,rig);
        return;
    }
    if(getPrec(str[ind]) == 2)
    {
        Node* left = getNode();
        left->val = "term";
        parse(left,l,ind-1);
        push(v,left);
        Node* op = getNode();
        op->val = str[ind];
        push(v,op);
        Node* rig = getNode();
        rig->val = "factor";
        parse(rig,ind+1,r);
        push(v,rig);
        return;
    }
    if(getPrec(str[ind]) == 3)
    {
        Node* left = getNode();
        left->val = "factor";
        parse(left,l,ind-1);
        push(v,left);
        Node* op = getNode();
        op->val = str[ind];
        push(v,op);
        Node* rig = getNode();
        rig->val = "operand";
        parse(rig,ind+1,r);
        push(v,rig);
        return;
    }
    return ;
}

int main()
{
    setPrec("==",1);
    setPrec(">",1);
    setPrec("*",3);
    setPrec("/",3);
    setPrec("+",2);
    setPrec("-",2);
    Node* tem = getNode();
    tem->val = "expr";
    parse(tem,0,sizeof(str)/sizeof(str[0])-1);
    //printf("%s",tem->next[0]->val);
    dfs(tem,NULL);
}