#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "const_language.h"
#include "const_in_reverse_front_end.h"
#include "operations_with_files.h"
#include "tree.h"
#include "list_of_func.h"
#include "name_table.h"
#include "local_name_table.h"
#include "write_program_from_tree.h"
#include "launch_reverse_front_end.h"

#define WRITE_NODE_BY_TYPE_(sign, value_type, name_print_func)  if (sign == value_type) {return name_print_func (node, tree_in_air);}

#define WRITE_ARITHMETIC_OPERATION_(code_of_operation, str_for_asm)        	\
	if (node -> value.value_keyword == code_of_operation)                  	\
	{                     													\
		fprintf (tree_in_asm -> file_for_asm, str_for_asm);					\
		break;																\
	}

#define WRITE_COMPARISON_OPERATION_(type_of_comparison,  str_with_comparison, str_with_jmp)           	\
	if (node -> value.value_keyword == type_of_comparison)           							 		\
	{																									\
		fprintf (tree_in_asm -> file_for_asm, str_with_comparison);										\
		fprintf (tree_in_asm -> file_for_asm, str_with_jmp);											\
																										\
		return NOT_ERROR;																				\
	}

static language_error_t write_node_in_file (node_t* node, reverse_t* tree_in_air);

static language_error_t write_constant_node                     (node_t* node, reverse_t* tree_in_air);
static language_error_t write_identifier_node                   (node_t* node, reverse_t* tree_in_air);
static language_error_t write_keyword_node                      (node_t* node, reverse_t* tree_in_air); 
static language_error_t write_function_definition_node          (node_t* node, reverse_t* tree_in_air);
static language_error_t write_parameters_in_file                (node_t* node, reverse_t* tree_in_air);
static language_error_t write_var_declaration_in_file           (node_t* node, reverse_t* tree_in_air);
static language_error_t write_call_in_file                      (node_t* node, reverse_t* tree_in_air);

static language_error_t write_arithmetic_operation_with_two_arg (node_t* node, reverse_t* tree_in_air);
static language_error_t write_arithmetic_operation_with_one_arg (node_t* node, reverse_t* tree_in_air);
static language_error_t write_if_while_in_file                  (node_t* node, reverse_t* tree_in_air);
static language_error_t write_comma_in_file                     (node_t* node, reverse_t* tree_in_air);
static language_error_t write_one_command_in_file               (node_t* node, reverse_t* tree_in_air);   
static language_error_t write_return_printf_in_file             (node_t* node, reverse_t* tree_in_air); 
static language_error_t write_operator_in_file                  (node_t* node, reverse_t* tree_in_air);  
static language_error_t write_assign_in_file                    (node_t* node, reverse_t* tree_in_air);

static language_error_t print_tabulation_in_file (reverse_t* tree_in_air);

//------------------------------------------------------------------------------------------------------------------------------

language_error_t write_program_from_tree (int argc, char** argv, node_t* node, char* str_with_table, 
                                          name_table_t* name_table, list_of_func_t* list_of_func)
{
	assert (str_with_table);
	assert (name_table);
	assert (list_of_func);
	assert (node);

	language_error_t status = NOT_ERROR;

	FILE* reverse_file = find_program_file (argc, argv, FIND_FILE_FOR_REVERSE);
	if (reverse_file == NULL) {return NOT_FIND_FILE_FOR_REVERSE;}

	reverse_t tree_in_air = {};

	tree_in_air.depth_of_tabulation = 0;
	tree_in_air.name_table          = name_table;
	tree_in_air.reverse_file        = reverse_file;
	tree_in_air.str_with_table      = str_with_table;
	tree_in_air.list_of_func        = list_of_func;

	status = write_node_in_file (node, &tree_in_air);
	if (status) {return status;}

	fclose (tree_in_air.reverse_file);
	
	return NOT_ERROR;
}

static language_error_t write_node_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);

	if (node == NULL) {return NOT_ERROR;}

	WRITE_NODE_BY_TYPE_(node -> type, CONSTANT,            write_constant_node);
	WRITE_NODE_BY_TYPE_(node -> type, IDENTIFIER,          write_identifier_node);
	WRITE_NODE_BY_TYPE_(node -> type, KEYWORD,             write_keyword_node);  
	WRITE_NODE_BY_TYPE_(node -> type, FUNCTION_DEFINITION, write_function_definition_node);  
	WRITE_NODE_BY_TYPE_(node -> type, PARAMETERS,          write_parameters_in_file);   
	WRITE_NODE_BY_TYPE_(node -> type, VAR_DECLARATION,     write_var_declaration_in_file);
	WRITE_NODE_BY_TYPE_(node -> type, CALL,                write_call_in_file);

	return NOT_ERROR;
	
}

static language_error_t write_constant_node (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	fprintf (tree_in_air -> reverse_file, " %lg ", node -> value.value_constant);

	return NOT_ERROR;
}

static language_error_t print_tabulation_in_file (reverse_t* tree_in_air)
{
	assert (tree_in_air);

	for (size_t index = 0; index < tree_in_air -> depth_of_tabulation; index++)
	{
		fprintf (tree_in_air -> reverse_file, "\t");
	}

	return NOT_ERROR;
}

static language_error_t write_identifier_node (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	size_t index_id_in_name_table = node -> value.value_identifier.index_id_in_name_table;

	name_t* name_id = &((tree_in_air -> name_table -> array_names)[index_id_in_name_table]);

	fprintf (tree_in_air -> reverse_file, " ");

	print_symbols_from_str_in_file (tree_in_air -> reverse_file, 
	                                tree_in_air -> str_with_table + name_id -> index_to_name_in_str, name_id -> len_name);

	fprintf (tree_in_air -> reverse_file, " ");

	return NOT_ERROR;
}

static language_error_t write_keyword_node (node_t* node, reverse_t* tree_in_air)       
{
	assert (tree_in_air);
	assert (node);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, ADD,   write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, SUB,   write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, MUL,   write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, DIV,   write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, DEG,   write_arithmetic_operation_with_two_arg);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, MORE,        write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, NO_MORE,     write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, LESS,        write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, NO_LESS,     write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, EQUALLY,     write_arithmetic_operation_with_two_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, NOT_EQUALLY, write_arithmetic_operation_with_two_arg);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, SIN,   write_arithmetic_operation_with_one_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, COS,   write_arithmetic_operation_with_one_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, SH,    write_arithmetic_operation_with_one_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, CH,    write_arithmetic_operation_with_one_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, SQRT,  write_arithmetic_operation_with_one_arg);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, LN,    write_arithmetic_operation_with_one_arg);
	//WRITE_NODE_BY_TYPE_(node -> value.value_keyword, DIFF,   write_arithmetic_operation_with_one_arg);
	//WRITE_NODE_BY_TYPE_(node -> value.value_keyword, FLOOR,   write_arithmetic_operation_with_one_arg);
	//LOG

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, IF,    write_if_while_in_file);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, WHILE, write_if_while_in_file);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, COMMA, write_comma_in_file);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, INT,   write_one_command_in_file);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, SCANF, write_one_command_in_file);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, ABORT, write_one_command_in_file);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, RETURN, write_return_printf_in_file);
	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, PRINTF, write_return_printf_in_file);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, OPERATOR, write_operator_in_file);

	WRITE_NODE_BY_TYPE_(node -> value.value_keyword, ASSIGN,   write_assign_in_file);

	return NOT_ERROR;
}

static language_error_t write_arithmetic_operation_with_two_arg (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_air -> reverse_file, " к ");

	status = write_node_in_file (node -> left, tree_in_air);
	if (status) {return status;}

	status = print_name_func_in_file_by_code (tree_in_air -> reverse_file, tree_in_air -> list_of_func, node -> value.value_keyword);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " ");

	status = write_node_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " лети ");

	return NOT_ERROR;
}

static language_error_t write_arithmetic_operation_with_one_arg (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	status = print_name_func_in_file_by_code (tree_in_air -> reverse_file, tree_in_air -> list_of_func, node -> value.value_keyword);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " к ");

	status = write_node_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " лети ");

	return NOT_ERROR;
}

static language_error_t write_if_while_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	status = print_name_func_in_file_by_code (tree_in_air -> reverse_file, tree_in_air -> list_of_func, node -> value.value_keyword);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " ");

	//fprintf (tree_in_air -> reverse_file, " к ");

	status = write_node_in_file (node -> left, tree_in_air);
	if (status) {return status;}

	//fprintf (tree_in_air -> reverse_file, " лети\n\n");

	fprintf (tree_in_air -> reverse_file, "\n\n");

	print_tabulation_in_file (tree_in_air);

	fprintf (tree_in_air -> reverse_file, "внимание!_взлёт\n");

	tree_in_air -> depth_of_tabulation += 1;

	status = write_node_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	tree_in_air -> depth_of_tabulation -= 1;
	print_tabulation_in_file (tree_in_air);

	fprintf (tree_in_air -> reverse_file, "успешная_посадка\n");

	return NOT_ERROR;
}

static language_error_t write_comma_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	status = write_node_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	if (node -> left != NULL)
	{
		fprintf (tree_in_air -> reverse_file, " и ");

		status = write_comma_in_file (node -> left, tree_in_air);
		if (status) {return status;}
	}

	return NOT_ERROR;
}

static language_error_t write_one_command_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	status = print_name_func_in_file_by_code (tree_in_air -> reverse_file, tree_in_air -> list_of_func, node -> value.value_keyword);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " ");

	return NOT_ERROR;
}

static language_error_t write_return_printf_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_air -> reverse_file, " ");

	status = print_name_func_in_file_by_code (tree_in_air -> reverse_file, tree_in_air -> list_of_func, node -> value.value_keyword);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " ");

	status = write_node_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	return status;
}

static language_error_t write_operator_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	print_tabulation_in_file (tree_in_air);

	status = write_node_in_file (node -> left, tree_in_air);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, "\n\n");

	if (node -> right != NULL)
	{
		status = write_operator_in_file (node -> right, tree_in_air);
		if (status) return status;
	}

	return NOT_ERROR;
}

static language_error_t write_function_definition_node (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_air -> reverse_file, "\n\nот_винта -------------------------------------------------------------------------\n\n");

	status = write_one_command_in_file (node -> left, tree_in_air);
	if (status) {return status;}

	status = write_identifier_node (node, tree_in_air);
	if (status) {return status;}

	status = write_parameters_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	return NOT_ERROR;
}

static language_error_t write_parameters_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_air -> reverse_file, " к ");

	if (node -> left != NULL && node -> left -> right != NULL)
	{
		status = write_comma_in_file (node -> left, tree_in_air);
		if (status) {return status;}
	}

	fprintf (tree_in_air -> reverse_file, " лети\n\n");

	print_tabulation_in_file (tree_in_air);

	fprintf (tree_in_air -> reverse_file, "внимание!_взлёт\n");

	tree_in_air -> depth_of_tabulation += 1;

	status = write_node_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	tree_in_air -> depth_of_tabulation -= 1;
	print_tabulation_in_file (tree_in_air);

	fprintf (tree_in_air -> reverse_file, "успешная_посадка\n");

	return NOT_ERROR;
}

static language_error_t write_var_declaration_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	status = write_one_command_in_file (node -> left, tree_in_air);
	if (status) {return status;}

	status = write_node_in_file (node -> right, tree_in_air);
	if (status) {return status;}

	return NOT_ERROR;
}

static language_error_t write_call_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	status = write_identifier_node (node -> right, tree_in_air);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " к ");

	if (node -> left != NULL)
	{
		status = write_comma_in_file (node -> left, tree_in_air);
		if (status) {return status;}
	}

	fprintf (tree_in_air -> reverse_file, " лети ");

	return NOT_ERROR;
}

static language_error_t write_assign_in_file (node_t* node, reverse_t* tree_in_air)
{
	assert (tree_in_air);
	assert (node);

	language_error_t status = NOT_ERROR;

	status = write_identifier_node (node -> right, tree_in_air);
	if (status) {return status;}

	status = print_name_func_in_file_by_code (tree_in_air -> reverse_file, tree_in_air -> list_of_func, node -> value.value_keyword);
	if (status) {return status;}

	fprintf (tree_in_air -> reverse_file, " ");

	status = write_node_in_file (node -> left, tree_in_air);
	if (status) {return status;}

	return NOT_ERROR;
}
