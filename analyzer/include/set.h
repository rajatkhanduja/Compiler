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

#define ALLOC_TREE() (tree_t *) malloc (sizeof (tree_t));

/* Function to insert element into the tree_t */
unsigned int insert_element (tree_t *root, state_t *addr);

/* Function to delete a tree */
void delete_tree (tree_t *root);

/* Function to take union of two sets, add elements from one tree to another */
void union_sets (tree_t *set1, tree_t *set2);
#endif // End of file
