/* This file contains the structures and functions for sets using binary search trees 
 * Author : Rajat Khanduja
 * 
 * The node of the search tree is the representative element of the set and tree ensures
 * log(n) search and insertion time. 
 */
#ifndef SET_H_INCLUDED
#define SET_H_INCLUDED

#include <fsm.h>

typedef struct tree_t 
{
	state_t *state_addr;
	struct tree_t *left;
	struct tree_t *right;
	unsigned int height;
} tree_t;

typedef struct list_t
{
	state_t *state_addr;
	struct list_t * next;
} list_t;

#define ALLOC_TREE(ptr) ptr = (tree_t *) malloc (sizeof (tree_t));\
			ptr->left = ptr->right = NULL; ptr->state_addr = NULL;
#define ALLOC_LIST(ptr) ptr = (list_t *) malloc (sizeof (list_t));\
			ptr->next = NULL;

/* Function to insert element into the tree_t */
unsigned int insert_element (tree_t *root, state_t *addr);

/* Function to find an element in the tree */
tree_t * find (tree_t *root, state_t *addr);

/* Function to delete a tree */
void delete_tree (tree_t *root);

/* Function to take union of two sets, add elements from one tree to another */
void union_sets (tree_t *set1, tree_t *set2);

/* Function to convert a tree to a list */
void tree_to_list (tree_t *tree, list_t *list);

/* Type of FOR_EACH 
 * void FOR_EACH (tree_t *, state_t * , EXPR)
 * The second element must be initialized before calling this. 
 * The EXPR, involves the alias (2nd argument) for state_addr of the tree. 
 */
#define FOR_EACH(tree,alias,EXPR) \
{\
	list_t *l;\
	ALLOC_LIST(l);\
	tree_to_list (tree, l);\
	while (l)\
	{\
		alias = l->state_addr;\
		EXPR;\
		l = l->next;\
	}\
}

#endif // End of file
