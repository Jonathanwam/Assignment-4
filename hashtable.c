#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node_ {
	char *key;
	void *value;
	struct node_ * next;
} node;

typedef struct hash_table_ {
    int size;
    struct node_ **table;
} hash_table;


hash_table *create_hash_table(int size)
{
    hash_table *t;

    t = malloc(sizeof(hash_table));
    t->table = malloc(sizeof(node *)*size);

    int i;
    for(i=0;i<size;i++)
    {
        t->table[i] = NULL;
    }

    t->size = size;
    return t;
}

int hash(hash_table *t, char *str)
{
    int val=0;
    while(*str!='\0')
    {
        val = *str+(val<<5)-val;
        str++;
    }
    val = val%t->size;
    if(val<0)
        val=val*-1;
    return val;
}

void hash_insert(hash_table *t, char *key, void *val)
{
    int idx = hash(t, key);

    node *n = t->table[idx];
    node *prev = NULL;

    while( n!=NULL && n->key != NULL && strcmp(key, n->key) > 0 )
    {
        prev = n;
        n = n->next;
    }

    if( n!=NULL && n->key != NULL && strcmp(key, n->key)==0 )
    {
        free(n->value);
        n->value = val;
    }
    else
    {
        node *newnode = malloc(sizeof(node));
        newnode->key = strdup(key);
        newnode->value = val;
        newnode->next = NULL;

        if(n==t->table[idx])
        {
            newnode->next = n;
            t->table[idx]=newnode;
        }
        else if(n==NULL)
        {
            prev->next = newnode;
        }
        else
        {
            newnode->next = n;
            prev->next = newnode;
        }

    }
}

void *hash_get(hash_table *t, char *key)
{
    int idx = hash(t, key);
    node * cur = t->table[idx];
    while(cur!=NULL && cur->key!=NULL && strcmp(key, cur->key) > 0)
    {
        cur=cur->next;
    }


    if(cur==NULL || cur->key==NULL || strcmp(key, cur->key) != 0)
        return NULL;
    else
        return cur->value;
}

void print_hash_table(hash_table *t)
{
    int i;
    struct node_* head = NULL;
    for(i=0;i<t->size;i++)
    {
        head = (struct node_ *)t->table[i];
        while(head!=NULL)
        {
            void *ret = hash_get(t, head->key);
            printf("%10d %s\n",*((int*)ret),head->key);
            head=head->next;
        }
    }
}


