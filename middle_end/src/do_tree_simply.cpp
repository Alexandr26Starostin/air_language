#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "const_language.h"
#include "const_in_middle_end.h"
#include "operations_with_files.h"
#include "tree.h"
#include "list_of_func.h"
#include "name_table.h"
#include "local_name_table.h"
#include "do_tree_simply.h"

#define change_node_(delete_branch, save_branch)                      \
	delete_node (node -> delete_branch);                              \
                                                                      \
	node -> delete_branch = NULL;                                     \
                                                                      \
	node_t* old_##save_branch = node -> save_branch;                  \
                                                                      \
	node -> type  = old_##save_branch -> type;                        \
	node -> value = old_##save_branch -> value;                       \
                                                                      \
	node -> left  = copy_tree (old_##save_branch -> left,  node);     \
	node -> right = copy_tree (old_##save_branch -> right, node);     \
                                                                      \
	delete_node (old_##save_branch);                                  \
                                                                      \
	count_of_simplification += 1;                                     \
                                                                      \
	return count_of_simplification;

#define COMPARE_CONSTANT_AND_NUMBER_(branch, number) (((node -> branch) -> type == CONSTANT) && (compare_doubles (((node -> branch) -> value).value_constant, number)))

static size_t           simplify_numbers          (node_t* node);
static size_t           simplify_neutral_elements (node_t* node);
static bool             compare_doubles           (double num_1, double num_2);
static double           evaluate_tree             (node_t* node);
static node_t*          copy_tree                 (node_t* old_node, node_t* parent_new_node);
static language_error_t count_vars                (node_t* node, size_t* ptr_quantity_vars);

//---------------------------------------------------------------------------------------------------------

language_error_t do_tree_simply (node_t* node)
{
	assert (node);

	size_t count_of_simplification = 0;

	while (true)      
	{
		count_of_simplification = 0;

		count_of_simplification += simplify_numbers          (node);
		count_of_simplification += simplify_neutral_elements (node);

		if (count_of_simplification == 0)
		{
			count_of_simplification += simplify_numbers          (node);
			count_of_simplification += simplify_neutral_elements (node);

			if (count_of_simplification == 0) 
			{
				return NOT_ERROR;
			}
		}
	}

	return NOT_ERROR;
}

static size_t simplify_numbers (node_t* node)
{
	assert (node);

	size_t count_of_simplification = 0;
	size_t quantity_vars           = 0;

	count_vars (node, &quantity_vars);

	if (quantity_vars == 0)
	{
		if (node -> type == CONSTANT) {return count_of_simplification;}
		
		(node -> value).value_constant = evaluate_tree (node);

		count_of_simplification += 1;
		
		node -> type = CONSTANT;

		delete_node (node -> left);
		delete_node (node -> right);

		node -> left  = NULL;
		node -> right = NULL;

		return count_of_simplification;
	}

	if (node -> left != NULL) {count_of_simplification += simplify_numbers (node -> left);}

	if (node -> right != NULL) {count_of_simplification += simplify_numbers (node -> right);}

	return count_of_simplification;
}

static size_t simplify_neutral_elements (node_t* node)
{
	assert (node);

	size_t count_of_simplification = 0;

	if (node -> left != NULL) {count_of_simplification += simplify_neutral_elements (node -> left);}

	if (node -> right != NULL) {count_of_simplification += simplify_neutral_elements (node -> right);}

	if (node -> type == KEYWORD)
	{
		if ((node -> value).value_keyword == ADD)
		{ 
			if (COMPARE_CONSTANT_AND_NUMBER_(left, 0)) {change_node_(left, right)}

			else if (COMPARE_CONSTANT_AND_NUMBER_(right, 0)) {change_node_(right, left)}
		}

		else if ((node -> value).value_keyword == SUB)
		{
			if (COMPARE_CONSTANT_AND_NUMBER_(left, 0))
			{
				(node -> value).value_keyword = MUL;

				((node -> left) -> value).value_constant = -1;

				count_of_simplification += 1;

				return count_of_simplification;
			}

			else if (COMPARE_CONSTANT_AND_NUMBER_(right, 0)) {change_node_(right, left)}
		}

		else if ((node -> value).value_keyword == MUL)
		{
			if (COMPARE_CONSTANT_AND_NUMBER_(left, 1)) {change_node_(left, right)}

			else if (COMPARE_CONSTANT_AND_NUMBER_(right, 1)) {change_node_(right, left)}

			else if (COMPARE_CONSTANT_AND_NUMBER_(left, 0) || COMPARE_CONSTANT_AND_NUMBER_(right, 0))
			{                                       
				delete_node (node -> left);
				delete_node (node -> right);

				node -> left  = NULL;
				node -> right = NULL;

				node -> type = CONSTANT;

				(node -> value).value_constant = 0;

				count_of_simplification += 1;

				return count_of_simplification;
			}
		}

		else if ((node -> value).value_keyword == DIV)
		{  
			if (COMPARE_CONSTANT_AND_NUMBER_(right, 1)) {change_node_(right, left)}

			if (COMPARE_CONSTANT_AND_NUMBER_(left, 0))
			{
				delete_node (node -> left);
				delete_node (node -> right);

				node -> left  = NULL;
				node -> right = NULL;

				node -> type = CONSTANT;

				(node -> value).value_constant = 0;

				count_of_simplification += 1;

				return count_of_simplification;
			}
		}
	}

	return count_of_simplification;
}

static bool compare_doubles (double num_1, double num_2)   //true if num_1 == num_2      false if num_1 != num_2
{
	if (abs (num_1 - num_2) < 0.0001) {return true;}

	else {return false;}
}

static double evaluate_tree (node_t* node)
{
	assert (node);

	switch (node -> type)
	{
		case CONSTANT: return (node -> value).value_constant;

		case KEYWORD:
		{
			switch ((node -> value).value_keyword)
			{
				case ADD: return evaluate_tree (node -> left) + evaluate_tree (node -> right);
				case SUB: return evaluate_tree (node -> left) - evaluate_tree (node -> right);
				case MUL: return evaluate_tree (node -> left) * evaluate_tree (node -> right);
				case DIV: return evaluate_tree (node -> left) / evaluate_tree (node -> right);
					
				case SIN:  return sin  (evaluate_tree (node -> right));
				case COS:  return cos  (evaluate_tree (node -> right));
				case SH:   return sinh (evaluate_tree (node -> right));
				case CH:   return cosh (evaluate_tree (node -> right));
				case SQRT: return sqrt (evaluate_tree (node -> right));
				case LN:   return log  (evaluate_tree (node -> right));

				case DEG:  return pow  (evaluate_tree (node -> left), evaluate_tree (node -> right));
				case LOG:  return log  (evaluate_tree (node -> right)) / log (evaluate_tree (node -> left));
				
				default:
				{
					printf ("Not find operation = %d", (node -> value).value_keyword);
					break;
				}
			}

			break;
		}

		default:
		{
			printf ("type = %d not find\n", node -> type);
			break;
		}
	}

	return 0;
}

static node_t* copy_tree (node_t* old_node, node_t* parent_new_node)
{
	if (old_node == NULL) {return NULL;}

	node_t* new_node = create_node (CONSTANT, 0, NULL, NULL, parent_new_node);
	if (new_node == NULL) {return NULL;}

	new_node -> type  = old_node -> type;
	new_node -> value = old_node -> value;
	
	if (old_node -> left != NULL)
	{	
		new_node -> left = copy_tree (old_node -> left, new_node);
		if (new_node -> left == NULL) {return NULL;}
	}

	if (old_node -> right != NULL)
	{
		new_node -> right = copy_tree (old_node -> right, new_node);
		if (new_node -> right == NULL) {return NULL;}
	}

	return new_node;
}

static language_error_t count_vars (node_t* node, size_t* ptr_quantity_vars)
{
	assert (node);
	assert (ptr_quantity_vars);

	if ((node -> type != CONSTANT) && ! ((node -> type == KEYWORD) && ((node -> value.value_keyword >= SIN && node -> value.value_keyword <= SQRT) ||
	 															       (node -> value.value_keyword >= SH  && node -> value.value_keyword <= DEG)))) 
		*ptr_quantity_vars += 1;

	if (node -> left != NULL)
		count_vars (node -> left, ptr_quantity_vars);

	if (node -> right != NULL)
		count_vars (node -> right, ptr_quantity_vars);

	return NOT_ERROR;
}
