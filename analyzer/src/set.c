/* File which includes the functions to implement a set using a tree */

#include <set.h>
#include <stdio.h>

unsigned int insert_element (tree_t *root, state_t *addr)
{
	if ( 0 == root->state_addr )
	{
		/* state_addr is zero implies it has not been used. */
		root->state_addr = addr;
		return 0;
	}
	if (addr > root->state_addr)
	{
		if (root->right)
		{
			root->height = 1 + insert_element (root->right, addr);
		}
		else
		{
			ALLOC_TREE(root->right);
			if (!root->right)
			{
				fprintf (stderr, "Unable to allocate space\n");
				exit (2);
			}

			root->right->left = root->right->right = NULL;
			root->right->state_addr = addr;
			root->right->height = 0;

			root->height++;
		}
	}
	else if (addr < root->state_addr)
	{
		if (root->left)
		{
			root->height = 1 + insert_element (root->left, addr);
		}
		else
		{
			ALLOC_TREE(root->left);
			if (!root->left)
			{
				fprintf (stderr, "Unable to allocate space\n");
				exit (2);
			}

			root->left->right = root->left->left = NULL;
			root->left->state_addr = addr;
			root->left->height = 0;

			root->height++;
		}
	}

	return root->height;
}

void delete_tree (tree_t *root)
{
	if ( root->left)
	{
		delete_tree (root->left);
	}

	if ( root->right)
	{
		delete_tree (root->right);
	}

	free (root);
}


tree_t * find ( tree_t * root, state_t * addr)
{
	if (addr == root->state_addr)
	{
		return root;
	}

	if ( root->left )
	{
		return (find (root->left, addr));
	}

	if ( root-> right )
	{
		return (find (root->right, addr));
	}
}

void union_sets_internal (tree_t *set1, tree_t *set2)
{
	/* This assumes that set1 is the larger out of the two and 
	 * we iterate over set2 to insert elements into set1 and delete set2
	 */
	
	insert_element (set1, set2->state_addr);

	if ( set2->left)
	{
		union_sets_internal (set1, set2->left);
	}

	if ( set2->right)
	{
		union_sets_internal (set1, set2->right);
	}

	free (set2);
}

void union_sets (tree_t *set1, tree_t *set2)
{
	/* insert the smaller tree into larger tree
	 * Make the smaller one 'set2' and the larger one 'set1'
	 */

	if ( set1->height< set2->height )
	{
		tree_t *tmp = set1;
		set1 = set2;
		set2 = tmp;
	}

	// Insert set2 into set1 and delete the tree
	union_sets_internal (set1, set2);
}

void tree_to_list (tree_t *tree, list_t *list)
{
	list->state_addr = tree->state_addr;
	
	list_t *tmp; 
	
	if ( tree->left )
	{
		ALLOC_LIST(tmp);
		list->next = tmp;
		tree_to_list (tree->left, tmp);
	}
	else if ( tree->right )
	{
		ALLOC_LIST(tmp);
		list->next = tmp;
		tree_to_list (tree->right, tmp);
	}
}
