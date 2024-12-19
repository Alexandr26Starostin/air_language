#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "const_language.h"
#include "list_of_func.h"
#include "tree.h"

static language_error_t print_edge                     (node_t* node, FILE* file_tree, dump_tree_t* tree_dump);
static language_error_t print_symbols_from_str_in_file (char* str, size_t count_of_symbols, FILE* file_tree);

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

language_error_t delete_node (node_t* node)
{
	if (node != NULL)
	{
		delete_node (node -> left);
		delete_node (node -> right);

		free (node);
	}

	return NOT_ERROR;
}

language_error_t dump_tree (node_t* node, dump_tree_t* tree_dump)
{
	assert (node);
	assert (tree_dump);

	FILE* file_tree = fopen ("tree.dot", "w");
	if (file_tree == NULL) {printf ("Not find diff.dot\n"); return NOT_FIND_TREE_DOT;}

	fprintf (file_tree, "digraph\n{\n\tnode[fontsize=9]\n\n\t");

	print_edge (node, file_tree, tree_dump);

	fprintf (file_tree, "}");

	fclose (file_tree);

	system (tree_dump -> str_for_system);

	fprintf (tree_dump -> tree_html, "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
						"<img src=\"pictures/diff_%c%c%c%c.svg\" style=\"width: 100%%\">\n", 
						(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 0], 
						(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 1], 
						(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 2], 
						(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 3]);

	tree_dump -> index_picture += 1;
	size_t copy_index = tree_dump -> index_picture;

	(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 0] = '0' + (char) (copy_index / 1000);
	copy_index                                 %= 1000;

	(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 1] = '0' + (char) (copy_index / 100);
	copy_index                                 %= 100;

	(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 2] = '0' + (char) (copy_index / 10);
	copy_index                                 %= 10;

	(tree_dump -> str_for_system)[INDEX_NUMBER_OF_PICTURE + 3] = '0' + (char) (copy_index / 1);
	copy_index                                 %= 1;

	return NOT_ERROR;
}

static language_error_t print_edge (node_t* node, FILE* file_tree, dump_tree_t* tree_dump)
{
	assert (file_tree);
	assert (tree_dump);

	if (node != NULL)
	{ 
		//printf ("node -> type == %d\n", node -> type);

		switch (node -> type)
		{
			case CONSTANT:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%lg| type = %d (CONSTANT) | {left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"#1AC6D9\"]\n\n\t", 
									node, (node -> value).value_constant, node -> type, node -> left, node, node -> parent, node -> right);
				break;

			case IDENTIFIER:
			{
				fprintf (file_tree, "node_%p [shape=record, label = \"{%ld (", 
									node, (node -> value).value_identifier.index_id_in_name_table);

				// printf ("\n\nindex_id_in_name_table == %ld\n", (node -> value).value_identifier.index_id_in_name_table);
				// printf ("index_to_name_in_str == %ld\n",     (name_table -> array_names)[(node -> value).value_identifier.index_id_in_name_table].index_to_name_in_str);
				// printf ("len_name == %ld\n\n",   (name_table -> array_names)[(node -> value).value_identifier.index_id_in_name_table].len_name);

				print_symbols_from_str_in_file (tree_dump -> str_with_program + (tree_dump -> name_table -> array_names)[(node -> value).value_identifier.index_id_in_name_table].index_to_name_in_str, 
				                                                   (tree_dump -> name_table -> array_names)[(node -> value).value_identifier.index_id_in_name_table].len_name, file_tree);

								
				fprintf (file_tree, ")| type = %d (IDENTIFIER) | {left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"#F0FF4F\"]\n\n\t", 
									node -> type, node -> left, node, node -> parent, node -> right);

				break;
			}
			
			case KEYWORD:
			{
				fprintf (file_tree, "node_%p [shape=record, label = \"{%d (", 
									node, (node -> value).value_keyword);

				print_name_func_in_file_by_code (file_tree, tree_dump -> list_of_func, (node -> value).value_keyword);

				fprintf (file_tree, ")| type = %d (KEYWORD) | {left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"#BDEF9E\"]\n\n\t", 
									node -> type, node -> left, node, node -> parent, node -> right);

				break;
			}
			
			case FUNCTION_DEFINITION:
			{
				fprintf (file_tree, "node_%p [shape=record, label = \"{%ld (", 
									node, (node -> value).value_function_definition);

				print_symbols_from_str_in_file (tree_dump -> str_with_program + (tree_dump ->name_table -> array_names)[(node -> value).value_function_definition].index_to_name_in_str, 
				                                                   (tree_dump -> name_table -> array_names)[(node -> value).value_function_definition].len_name, file_tree);

				fprintf (file_tree, ")| type = %d (FUNCTION_DEFINITION) | {left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"#68F10D\"]\n\n\t", 
									node -> type, node -> left, node, node -> parent, node -> right);
				break;
			}
			
			case PARAMETERS:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%lg| type = %d (PARAMETERS) | {left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"#FF91DA\"]\n\n\t", 
									node, (node -> value).value_parameters, node -> type, node -> left, node, node -> parent, node -> right);
				break;
			
			case VAR_DECLARATION:
			{
				fprintf (file_tree, "node_%p [shape=record, label = \"{%ld (", 
									node, (node -> value).value_var_declaration);
								
				print_symbols_from_str_in_file (tree_dump -> str_with_program + (tree_dump -> name_table -> array_names)[(node -> value).value_var_declaration].index_to_name_in_str, 
				                                                   (tree_dump -> name_table -> array_names)[(node -> value).value_var_declaration].len_name, file_tree);

				fprintf (file_tree, ")| type = %d (VAR_DECLARATION) | {left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"#B93CDC\"]\n\n\t", 
									node -> type, node -> left, node, node -> parent, node -> right);

				break;
			}
			
			case CALL:
				fprintf (file_tree, "node_%p [shape=record, label = \"{%lg| type = %d (CALL) | {left = %p | node = %p | parent = %p | right = %p}}\" "
									"style=\"filled\",fillcolor=\"#E33A3A\"]\n\n\t", 
									node, (node -> value).value_call, node -> type, node -> left, node, node -> parent, node -> right);
				break;

			default: break;
		}

		if (node -> parent != NULL)
		{
			fprintf (file_tree, "edge[color=\"black\",fontsize=12]\n\tnode_%p -> node_%p\n\n\t", node -> parent, node);
		}

		print_edge (node -> left,  file_tree, tree_dump);
		print_edge (node -> right, file_tree, tree_dump);
	}
		
	return NOT_ERROR;
}

static language_error_t print_symbols_from_str_in_file (char* str, size_t count_of_symbols, FILE* file_tree)
{
	assert (str);
	assert (file_tree);

	//printf ("begin print_symbols_from_str_in_file\n");

	for (size_t index = 0; index < count_of_symbols; index++)
	{	
		//printf ("%c", str[index]);
		fprintf (file_tree, "%c", str[index]);
	}

	return NOT_ERROR;
}
