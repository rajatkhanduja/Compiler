#include <hashtable.rxg.h>

/*
 * st_ninstances() takes in a pointer to a
 * symbol table entry and returns the number
 * of times the token contained by it has occured
 * so far.
 * On error, a negative integer is returned.
 */
int st_ninstances(ste*);

/*
 * st_find() accepts token name and finds the
 * symbol table entry (via hash table) containing
 * the requested token name. On success, it returns
 * a pointer to the corresponding symbol table entry
 * while on failure, or in case of an error, NULL
 * is returned.
 */
ste* st_find(char*);


/*
 * st_link() accepts a pointer to hash linked list node
 * and a pointer to symbol table entry. It links the
 * symbol table entry to the node of the hash linked
 * list token type in the hash table, so that st_find()
 * has fast accesses.
 *
 * static definition in the file:
 * symboltable_functions.rb.c
 */
//void st_link(ste*);


/*
 * st_insert() takes in pointer to the head of the
 * original symbol table, token name, token type and
 * the line number the token occurs in.
 * It then looks in the symbol table (using st_find())
 * whether the requested token name is already in the
 * symbol table or not; if found, the corresponding
 * symbol table entry's linenumber will be updated
 * else a new node of struct symbol_table_entry will
 * be created and a pointer to it will be returned
 * after inserting it in the front of the original
 * list. On error, it returns NULL.
 *
 * Thus, make sure to catch the value returned by
 * the function in the head of the list and also
 * make sure that the head is not of the type:
 * ste* const.
 */
ste* st_insert(ste*, char*, int, int);


/*
 * st_printentry() takes in a pointer to a symbol
 * table entry and a file pointer. It prints the
 * contents of the symbol table entry in a
 * formatted way in the file.
 */
inline void st_printentry(ste*, FILE*);

/*
 * st_printtable() takes in a pointer to the head
 * of the symbol table and a file pointer. It
 * prints the contents of the symbol table in a
 * formatted way in the file.
 */
void st_printtable(ste*, FILE*);

