#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "const_in_front_end.h"
#include "tree.h"

static front_end_error_t print_edge (node_t* node, FILE* file_tree);

//---------------------------------------------------------------------------------------------------------------------------

node_t* create_node (node_type_t type, double value, node_t* left, node_t* right, node_t* parent)
{
	node_t* new_node = (node_t*) calloc (1, sizeof (node_t));
	if (new_node == NULL)
	{
		printf ("\nError in %s:%d\nNot memory for new_node\n\n", __FILE__, __LINE__);
		return NULL;
	}

	new_node -> type  = type;
	new_node -> left  = left;
	new_node -> right = right;

	if (parent != NULL) {new_node -> parent = parent;}
	if (left   != NULL) {left     -> parent = new_node;}
	if (right  != NULL) {right    -> parent = new_node;}

	switch (type)
	{
		case CONSTANT:
			(new_node -> value).value_constant = value;
			break;
		
		case IDENTIFIER:
			(new_node -> value).value_identifier.index_id_in_name_table = (size_t) value;
			break;
		
		case KEYWORD:
			(new_node -> value).value_keyword = (base_func_t) value;
			break;
		
		case FUNCTION_DEFINITION:
			(new_node -> value).value_function_definition = (size_t) value;
			break;
		
		case PARAMETERS:
			(new_node -> value).value_parameters = NAN;
			break;
		
		case VAR_DECLARATION:
			(new_node -> value).value_var_declaration = (size_t) value;
			break;
		
		case CALL:
			(new_node -> value).value_call = NAN;
			break;
		
		default: break;
	}

	return new_node;
}

front_end_error_t delete_node (node_t* node)
{
	if (node != NULL)
	{
		delete_node (node -> left);
		delete_node (node -> right);

		free (node);
	}

	return NOT_ERROR;
}

front_end_error_t dump_tree (node_t* node, char* str_for_system, size_t* ptr_index_picture, FILE* tree_html)
{
	assert (node);
	assert (str_for_system);
	assert (ptr_index_picture);
	assert (tree_html);

	FILE* file_tree = fopen ("tree.dot", "w");
	if (file_tree == NULL) {printf ("Not find diff.dot\n"); return NOT_FIND_TREE_DOT;}

	fprintf (file_tree, "digraph\n{\n\tnode[fontsize=9]\n\n\t");

	print_edge (node, file_tree);

	fprintf (file_tree, "}");

	fclose (file_tree);

	system (str_for_system);

	fprintf (tree_html, "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
						"<img src=\"pictures/diff_%c%c%c%c.svg\" style=\"width: 100%%\">\n", 
						str_for_system[INDEX_NUMBER_OF_PICTURE + 0], 
						str_for_system[INDEX_NUMBER_OF_PICTURE + 1], 
						str_for_system[INDEX_NUMBER_OF_PICTURE + 2], 
						str_for_system[INDEX_NUMBER_OF_PICTURE + 3]);

	*ptr_index_picture += 1;
	size_t copy_index = *ptr_index_picture;

	str_for_system[INDEX_NUMBER_OF_PICTURE + 0] = '0' + (char) (copy_index / 1000);
	copy_index                                 %= 1000;

	str_for_system[INDEX_NUMBER_OF_PICTURE + 1] = '0' + (char) (copy_index / 100);
	copy_index                                 %= 100;

	str_for_system[INDEX_NUMBER_OF_PICTURE + 2] = '0' + (char) (copy_index / 10);
	copy_index                                 %= 10;

	str_for_system[INDEX_NUMBER_OF_PICTURE + 3] = '0' + (char) (copy_index / 1);
	copy_index                                 %= 1;

	return NOT_ERROR;
}

static front_end_error_t print_edge (node_t* node, FILE* file_tree)
{
	assert (file_tree);

	if (node != NULL)
	{ 
		switch (node -> type)
		{
			case CONSTANT:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%lg| {type = %d (CONSTANT) | left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"blue\"]\n\n\t", 
									node, (node -> value).value_constant, node -> type, node -> left, node, node -> parent, node -> right);
				break;

			case IDENTIFIER:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%ld| {type = %d (IDENTIFIER) | left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"blue\"]\n\n\t", 
									node, (node -> value).value_identifier.index_id_in_name_table, node -> type, node -> left, node, node -> parent, node -> right);
				break;
			
			case KEYWORD:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%d| {type = %d (KEYWORD) | left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"blue\"]\n\n\t", 
									node, (node -> value).value_keyword, node -> type, node -> left, node, node -> parent, node -> right);
				break;
			
			case FUNCTION_DEFINITION:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%ld| {type = %d (FUNCTION_DEFINITION) | left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"blue\"]\n\n\t", 
									node, (node -> value).value_function_definition, node -> type, node -> left, node, node -> parent, node -> right);
				break;
			
			case PARAMETERS:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%lg| {type = %d (PARAMETERS) | left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"blue\"]\n\n\t", 
									node, (node -> value).value_parameters, node -> type, node -> left, node, node -> parent, node -> right);
				break;
			
			case VAR_DECLARATION:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%ld| {type = %d (VAR_DECLARATION) | left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"blue\"]\n\n\t", 
									node, (node -> value).value_function_definition, node -> type, node -> left, node, node -> parent, node -> right);
				break;
			
			case CALL:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%lg| {type = %d (PARAMETERS) | left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"blue\"]\n\n\t", 
									node, (node -> value).value_call, node -> type, node -> left, node, node -> parent, node -> right);
				break;

			default: break;
		}

		if (node -> parent != NULL)
		{
			fprintf (file_tree, "edge[color=\"black\",fontsize=12]\n\tnode_%p -> node_%p\n\n\t", node -> parent, node);
		}

		print_edge (node -> left,  file_tree);
		print_edge (node -> right, file_tree);

	}
		
	return NOT_ERROR;
}

