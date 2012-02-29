#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"
#include "token.h"

#define EOS '\0'

int hashpjw(const char *s)
{
        const char *p = s;
        unsigned h = 0, g;
        /* for each character c, shift the 
           bits of h left 4 positions and add in c.     
           if any of the four high-order bits of h
           is 1, shift the four bits right 24 positions, 
           exclusive-or them into h, and reset to 0 
           any of the four high-order bits that was 1 */
        for(;*p != EOS; p = p + 1) {
                h = (h << 4) + (*p);
                if(g = h&0xf0000000) {
                        h = h ^ (g >> 24);
                        h = h ^ g;
                }
        }
        return h % S.size;
}

/* initialize symbol table with reserved key words */
void initialize()
{

        int idx;
        S.size = 211; /* prime */
        int hashidx;

        int num_keywords = 12;
        
        const char *string[12] = {"if", "int", "declarations", "enddeclarations",
                          "class", "method", "program", "val", "while",
                          "else", "return", "void"};
        int types[12] = {IFnum, INTnum, DECLARATIONnum, ENDDECLARATIONSnum,
                        CLASSnum, METHODnum, PROGRAMnum, VALnum, WHILEnum, 
                        ELSEnum, RETURNnum, VOIDnum};
                
        struct node *tmp;

        /* allocate array of pointers */
        S.hash_array = malloc(S.size*sizeof(struct node*));
        
        /* zero-fill all pointers in table */
        for(idx = 0; idx < S.size; idx++)
                S.hash_array[idx] = 0;

        /* now hash each keyword and insert into hash table */
        for(idx = 0; idx < num_keywords; idx++) {
                /* hash keyword */
                hashidx = hashpjw(string[idx]);

                /* create temporary node */
                tmp = malloc(sizeof(struct node));

                /* link node into symbol table */
                tmp->next = S.hash_array[hashidx];
                tmp->type = types[idx];
                strcpy(tmp->lexeme.string, string[idx]);

                S.hash_array[hashidx] = tmp;
        }
        
}

/* insert a string literal into the hash table */
struct node* insertString(const char *str) 
{

        int hashidx;

        /* hash the string literal */
        hashidx = hashpjw(str);

        /* allocate a temporary node to hold the string literal */
        struct node *tmp = malloc(sizeof(struct node));

        /* link the node into the correct hash list using
           the hash index just created */ 
        tmp->next = S.hash_array[hashidx];
        tmp->type = SCONSTnum; /* set type of token to string constant */
        strcpy(tmp->lexeme.string, str); /* copy string literal into node */
        S.hash_array[hashidx] = tmp;

        return tmp; /* return pointer to node */
}

/* insert an identifier into the hash table */
struct node* insertID(const char *ident)
{
        int len;
        int idx;
        char *str;
        len = strlen(ident);

        str = malloc(len*sizeof(char));
        for(idx = 0; idx < len; idx++)
                str[idx] = tolower(ident[idx]);

        str[len] = '\0';


        int hashidx;
        /* hash the identifer name */
        hashidx = hashpjw(str);

        /* allocate a temporary node to hold the identifier */
        struct node *tmp = malloc(sizeof(struct node));

        /* link the node into the correct hash list using
           the hash index just created */ 
        tmp->next = S.hash_array[hashidx];
        tmp->type = IDnum;  /* set type of token to IDnum for identifier */
        strcpy(tmp->lexeme.string, str); /* copy identifier name into node */
        S.hash_array[hashidx] = tmp;

        free(str);

        return tmp; /* return pointer to node */
}

/* search for a given name in the hash table */
struct node* search(const char *str)
{
        int len;
        int idx;
        char *strstring;
        int hashidx;
        struct node *ptr;       

        len = strlen(str);

        strstring = malloc(len*sizeof(char));

        for(idx = 0; idx < len; idx++)
                strstring[idx] = tolower(str[idx]);

        strstring[len] = '\0';

        /* hash the name */
        hashidx = hashpjw(strstring);

        /* go to the array entry that corresponds to the index value
           that was just hashed.  Then, traverse the linked list that
           this array entry points to.  Thus, search for the name
           in the hash chain.  When we find it, return a pointer to the
           node that contains it.  Otherwise, return 0(the NULL pointer) */
        for(ptr = S.hash_array[hashidx]; ptr != 0; ptr = ptr->next) {
                if(strcmp(strstring, ptr->lexeme.string) == 0)
                        return ptr;
        }

        free(strstring);
        
        return 0;
}
