#ifndef HASH_H
#define HASH_H

/* hash table with separate chaining */
struct node
{
        int type; /* token type */
        int len;
        union 
        {
                int val;  
                char string[80]; /* lexeme string */
        } lexeme;

        struct node *next;
        struct node *prev;
};

struct Table
{
        int size; /* size of hash table */
        /* the hash table: an array of pointers */
        struct node **hash_array;
};

struct Table S;
        
/* insert identifier into symbol table */
struct node* 
insertID(const char *ident);

/* insert string literal into symbol table */
struct node*
insertString(const char *str);

/* TODO */
struct node*
insertInt();

/* function to initialize symbol table with reserved words */
void
initialize();

/* search symbol table for a given name; either an identifier or a string literal */
struct node*
search(const char *str);

#endif
 