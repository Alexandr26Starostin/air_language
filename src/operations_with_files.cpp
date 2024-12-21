#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "const_language.h"
#include "name_table.h"
#include "tree.h"
#include "local_name_table.h"
#include "operations_with_files.h"

#define PRINT_EXAMPLE_OF_USING_FRONT_END_																			\
	printf ("Example of using front_end:\n"																			\
			"./front_end -read_program <name_program> " 															\
			"-read_tree <old_name_tree> -read_table <old_name_table> "												\
			"-write_tree <new_name_tree> -write_table <new_name_table>\n\n");

#define FIND_FILE_(operation_with_file, flag, mode)                                                                           	\
		case operation_with_file:                                                                                             	\
		{      																													\
			for (size_t index = 0; index < (size_t) argc; index++)																\
			{																													\
				if (strcmp (argv[index], flag) == 0)																			\
				{																												\
					name_file = argv[index + 1];																				\
					break;																										\
				}																												\
			}																													\
																																\
			if (name_file == NULL)																								\
			{																													\
				printf ("\n\nError in %s:%d\n'find_program_file' not find flag == \"%s\"\n\n", __FILE__, __LINE__, flag);		\
				PRINT_EXAMPLE_OF_USING_FRONT_END_;        																		\
																																\
				return NULL;																									\
			}																													\
																																\
			FILE* file = fopen (name_file, mode);																				\
			if (file == NULL)																									\
			{																													\
				printf ("\n\nError in %s:%d\n"																					\
						"'find_program_file' not find file, which have name: \"%s\"\n\n", __FILE__, __LINE__, name_file);		\
				PRINT_EXAMPLE_OF_USING_FRONT_END_;																				\
																																\
				return NULL;																									\
			}																													\
																																\
			return file;																										\
		}

static size_t           count_len_file                          (FILE* file);
static language_error_t print_node_in_file                      (FILE* file_for_tree, node_t* node);
static language_error_t print_name_table_in_file                (FILE* file_for_table, name_table_t* name_table, char* str_with_program);
static language_error_t print_list_of_local_name_tables_in_file (FILE* file_for_table, list_of_local_name_tables_t* list_of_local_name_tables);
static language_error_t print_local_name_tables_in_file         (FILE* file_for_table, local_name_table_t* local_name_table);
static language_error_t skip_spaces                             (char* str_with_tree, size_t* ptr_index_in_str);
static language_error_t create_local_table_from_str             (char*   str_with_table, list_of_local_name_tables_t* list_of_local_name_tables, 
                                                                 size_t* ptr_index_in_str, size_t quantity_of_id, long scope);

//-----------------------------------------------------------------------------------------------------------------------------------------

language_error_t read_file_to_str (int argc, char** argv, char** ptr_str, operation_with_file_t operation_with_file, language_error_t error_of_search)
{
	assert (argv);
	assert (ptr_str);

	FILE* file = find_program_file (argc, argv, operation_with_file);
	if (file == NULL) {return error_of_search;}

	size_t len_file = count_len_file (file);

	*ptr_str = (char*) calloc (len_file + 1, sizeof (char));
	if (*ptr_str == NULL)
	{
		printf ("\n\nError in %s:%d\n"
		        "not memory for str\n\n", __FILE__, __LINE__);

		fclose (file);

		return NOT_MEMORY_FOR_STR;
	}

	fread (*ptr_str, sizeof (char), len_file, file);

	fclose (file);

	return NOT_ERROR;
}

FILE* find_program_file (int argc, char** argv, operation_with_file_t operation_with_file)
{
	assert (argv);

	char* name_file = NULL;

	switch (operation_with_file)
	{
		FIND_FILE_(FIND_FILE_WITH_PROGRAM, "-read_program",  "r");
		FIND_FILE_(FIND_FILE_WITH_TREE,    "-read_tree",     "r");
		FIND_FILE_(FIND_FILE_WITH_TABLE,   "-read_table",    "r");
		FIND_FILE_(FIND_FILE_FOR_TREE,     "-write_tree",    "w");
		FIND_FILE_(FIND_FILE_FOR_TABLE,    "-write_table",   "w"); 
		FIND_FILE_(FIND_FILE_FOR_ASM,      "-write_asm",     "w");
		FIND_FILE_(FIND_FILE_FOR_REVERSE,  "-write_reverse", "w");
		
		default:
			PRINT_EXAMPLE_OF_USING_FRONT_END_;
			return NULL;
	}
}

static size_t count_len_file (FILE* file)      
{ 
    assert (file);

    fseek (file, 0, SEEK_END);
    size_t count_memory = ftell (file);
    fseek (file, 0, SEEK_SET);

    return count_memory;
}

language_error_t write_tree_in_file (int argc, char** argv, node_t* root_node)
{
	assert (argv);
	assert (root_node);

	FILE* file_for_tree = find_program_file (argc, argv, FIND_FILE_FOR_TREE);
	if (file_for_tree == NULL) {return NOT_FIND_FILE_FOR_TREE;}

	print_node_in_file (file_for_tree, root_node);

	fclose (file_for_tree);

	return NOT_ERROR;
}

static language_error_t print_node_in_file (FILE* file_for_tree, node_t* node)
{
	assert (file_for_tree);

	if (node != NULL && node -> type == KEYWORD && (node -> value).value_keyword == OPERATOR && node -> left == NULL)
	{
		fprintf (file_for_tree, "_ ");
		return NOT_ERROR;
	}

	if (node != NULL && node -> type == KEYWORD && (node -> value).value_keyword == COMMA && node -> right == NULL)
	{
		fprintf (file_for_tree, "_ ");
		return NOT_ERROR;
	}

	if (node == NULL)
	{
		fprintf (file_for_tree, "_ ");
		return NOT_ERROR;
	} 

	fprintf (file_for_tree, "( %d ", node -> type);

	switch (node -> type)
	{
		case CONSTANT:
			fprintf (file_for_tree, "%lg ", (node -> value).value_constant);
			break;

		case IDENTIFIER:
			fprintf (file_for_tree, "%ld ", (node -> value).value_identifier.index_id_in_name_table);
			break;

		case KEYWORD:
			fprintf (file_for_tree, "%d ", (node -> value).value_keyword);
			break;

		case FUNCTION_DEFINITION:
			fprintf (file_for_tree, "%ld ", (node -> value).value_function_definition);
			break;

		case PARAMETERS:
			break;

		case VAR_DECLARATION:
			fprintf (file_for_tree, "%ld ", (node -> value).value_var_declaration);
			break;
	

		case CALL:
			break;
		
		default: break;
	}

	print_node_in_file (file_for_tree, node -> left);
	print_node_in_file (file_for_tree, node -> right);

	fprintf (file_for_tree, ") ");

	return NOT_ERROR;
}

language_error_t write_table_in_file (int argc, char** argv, list_of_local_name_tables_t* list_of_local_name_tables, name_table_t* name_table, char* str_with_program)
{
	assert (argv);
	assert (list_of_local_name_tables);
	assert (name_table);
	assert (str_with_program);

	FILE* file_for_table = find_program_file (argc, argv, FIND_FILE_FOR_TABLE);
	if (file_for_table == NULL) {return NOT_FIND_FILE_FOR_TABLE;}

	print_name_table_in_file (file_for_table, name_table, str_with_program);

	fprintf (file_for_table, "%ld\n\n", list_of_local_name_tables -> free_index_in_list);

	print_list_of_local_name_tables_in_file (file_for_table, list_of_local_name_tables);

	fclose (file_for_table);

	return NOT_ERROR;
}

static language_error_t print_name_table_in_file (FILE* file_for_table, name_table_t* name_table, char* str_with_program)
{
	assert (file_for_table);
	assert (name_table);
	assert (str_with_program);

	fprintf (file_for_table, "%ld\n", name_table -> free_index);

	for (size_t index = 0; index < name_table -> free_index; index++)
	{
		print_symbols_from_str_in_file (file_for_table, str_with_program + (name_table -> array_names)[index].index_to_name_in_str, 
			                                                               (name_table -> array_names)[index].len_name);

		fprintf (file_for_table, "\n");
	}

	fprintf (file_for_table, "\n");

	return NOT_ERROR;
}

static language_error_t print_list_of_local_name_tables_in_file (FILE* file_for_table, list_of_local_name_tables_t* list_of_local_name_tables)
{
	assert (file_for_table);
	assert (list_of_local_name_tables);

	local_name_table_t* local_name_table = NULL;

	for (size_t index = 0; index < list_of_local_name_tables -> free_index_in_list; index++)
	{
		local_name_table = &((list_of_local_name_tables -> array_of_local_name_table)[index]);

		print_local_name_tables_in_file (file_for_table, local_name_table);
	}

	return NOT_ERROR;
}

static language_error_t print_local_name_tables_in_file (FILE* file_for_table, local_name_table_t* local_name_table)
{
	assert (file_for_table);
	assert (local_name_table);

	fprintf (file_for_table, "%ld %ld\n", local_name_table -> free_index_in_local_name_table, 
		                                  local_name_table -> scope_of_local_name_table);

	for (size_t index = 0; index < local_name_table -> free_index_in_local_name_table; index++)
	{
		//(local_name_table -> array_of_local_names)[index]

		fprintf (file_for_table, "%ld %d\n", (local_name_table -> array_of_local_names)[index].index_id_in_name_table, 
		                                     (local_name_table -> array_of_local_names)[index].type_id); 
	}

	fprintf (file_for_table, "\n\n");

	return NOT_ERROR;
}

language_error_t create_tree_from_str (char* str_with_tree, node_t** ptr_node, size_t* ptr_index_in_str)
{
	assert (str_with_tree);
	assert (ptr_node);
	assert (ptr_index_in_str);

	language_error_t status = NOT_ERROR;

	int shift_in_str = 0;
	
	skip_spaces (str_with_tree, ptr_index_in_str);
	if (str_with_tree[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TREE_FROM_STR;}

	if (str_with_tree[*ptr_index_in_str] != '(')
	{
		printf ("Error in %s:%d\n'create_tree_from_str' wait '(' in index_str = %ld, but find it: %c\n\n", 
			__FILE__, __LINE__, *ptr_index_in_str, str_with_tree[*ptr_index_in_str]);
		return ERROR_IN_CREATE_TREE_FROM_STR;
	}

	*ptr_index_in_str += 1;  //skip '(' 

	skip_spaces (str_with_tree, ptr_index_in_str);
	if (str_with_tree[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TREE_FROM_STR;}

	int number_of_type = 0;

	sscanf (str_with_tree + *ptr_index_in_str, "%d%n", &number_of_type, &shift_in_str);

	//printf ("shift_in_str == %d\n", shift_in_str);

	node_type_t type = (node_type_t) number_of_type;

	*ptr_index_in_str += shift_in_str;

	skip_spaces (str_with_tree, ptr_index_in_str);
	if (str_with_tree[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TREE_FROM_STR;}

	double value_node = 0;

	if (type != PARAMETERS && type != CALL)
	{
		sscanf (str_with_tree + *ptr_index_in_str, "%lg%n", &value_node, &shift_in_str);

		//printf ("shift_in_str == %d\n", shift_in_str);

		*ptr_index_in_str += shift_in_str;

		skip_spaces (str_with_tree, ptr_index_in_str);
		if (str_with_tree[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TREE_FROM_STR;}
	}

	*ptr_node = create_node (type, value_node, NULL, NULL, NULL);
	if (*ptr_node == NULL) {printf ("Error in %s:%d", __FILE__, __LINE__); return NOT_MEMORY_FOR_NEW_NODE;}

	//-----------------------------------------------------------------------------------------------------------------

	if (str_with_tree[*ptr_index_in_str] == '(')
	{
		status = create_tree_from_str (str_with_tree, &((*ptr_node) -> left), ptr_index_in_str);
		if (status) {return status;}

		(*ptr_node) -> left -> parent = *ptr_node;
	}

	else
	{
		*ptr_index_in_str += 1;  // skip '_'
	}

	skip_spaces (str_with_tree, ptr_index_in_str);
	if (str_with_tree[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TREE_FROM_STR;}

	if (str_with_tree[*ptr_index_in_str] == '(')
	{
		status = create_tree_from_str (str_with_tree, &((*ptr_node) -> right), ptr_index_in_str);
		if (status) {return status;}

		(*ptr_node) -> right -> parent = *ptr_node;
	}

	else   //str_with_tree[*ptr_index_in_str] == '_'
	{
		*ptr_index_in_str += 1;  // skip '_'
	}

	skip_spaces (str_with_tree, ptr_index_in_str);
	if (str_with_tree[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TREE_FROM_STR;}

	if (str_with_tree[*ptr_index_in_str] != ')')
	{
		printf ("Error in %s:%d\n'create_tree_from_str' wait ')' in index_str = %ld, but find it: %c\n\n", 
			__FILE__, __LINE__, *ptr_index_in_str, str_with_tree[*ptr_index_in_str]);
		return ERROR_IN_CREATE_TREE_FROM_STR;
	}

	*ptr_index_in_str += 1;    //skip ')'

	return NOT_ERROR;
}

static language_error_t skip_spaces (char* str_with_tree, size_t* ptr_index_in_str)
{
	assert (str_with_tree);
	assert (ptr_index_in_str);

	while (str_with_tree[*ptr_index_in_str] == ' ' || str_with_tree[*ptr_index_in_str] == '\n' || str_with_tree[*ptr_index_in_str] == '\t')
	{
		*ptr_index_in_str += 1;
	}

	return NOT_ERROR;
}

language_error_t create_table_from_str (char* str_with_table, name_table_t* name_table, size_t* ptr_index_in_str)
{
	assert (str_with_table);
	assert (name_table);
	assert (ptr_index_in_str);

	language_error_t status = NOT_ERROR;

	char word_from_str[MAX_LEN_WORD_FROM_STR] = "\0";

	size_t quantity_of_names = 0;
	int    shift_in_str      = 0;

	sscanf (str_with_table + *ptr_index_in_str, "%ld%n", &quantity_of_names, &shift_in_str);

	*ptr_index_in_str += shift_in_str;

	skip_spaces (str_with_table, ptr_index_in_str);
	if (str_with_table[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TABLE_FROM_STR;}

	for (size_t index_name = 0; index_name < quantity_of_names; index_name++)
	{
		sscanf (str_with_table + *ptr_index_in_str, "%s%n", word_from_str, &shift_in_str);
		
		status = add_name_in_table (name_table, *ptr_index_in_str, shift_in_str, str_with_table);
		if (status) {return status;}

		*ptr_index_in_str += shift_in_str;

		skip_spaces (str_with_table, ptr_index_in_str);
		if (str_with_table[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_TABLE_FROM_STR;}
	}

	return NOT_ERROR;
}

language_error_t create_list_of_local_tables_from_str (char* str_with_table, list_of_local_name_tables_t* list_of_local_name_tables, size_t* ptr_index_in_str)
{
	assert (str_with_table);
	assert (list_of_local_name_tables);
	assert (ptr_index_in_str);

	language_error_t status = NOT_ERROR;

	size_t quantity_of_scopes = 0;
	int    shift_in_str       = 0;
	size_t quantity_of_id     = 0;
	long   scope              = 0;

	sscanf (str_with_table + *ptr_index_in_str, "%ld%n", &quantity_of_scopes, &shift_in_str);
	*ptr_index_in_str += shift_in_str;

	skip_spaces (str_with_table, ptr_index_in_str);
	if (str_with_table[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_LIST_OF_LOCAL_TABLES_FROM_STR;}

	for (size_t index_scope = 0; index_scope < quantity_of_scopes; index_scope++)
	{
		sscanf (str_with_table + *ptr_index_in_str, "%ld%n", &quantity_of_id, &shift_in_str);
		*ptr_index_in_str += shift_in_str;

		skip_spaces (str_with_table, ptr_index_in_str);
		if (str_with_table[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_LIST_OF_LOCAL_TABLES_FROM_STR;}

		//------------------------------------------------------------------------------------------------------------------

		sscanf (str_with_table + *ptr_index_in_str, "%ld%n", &scope, &shift_in_str);
		*ptr_index_in_str += shift_in_str;

		skip_spaces (str_with_table, ptr_index_in_str);
		if (str_with_table[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_LIST_OF_LOCAL_TABLES_FROM_STR;}

		//------------------------------------------------------------------------------------------------------------------

		status = add_new_local_name_table_in_list (list_of_local_name_tables, scope);
		if (status) {return status;}

		status = create_local_table_from_str (str_with_table, list_of_local_name_tables, ptr_index_in_str, quantity_of_id, scope);
		if (status) {return status;}
	}

	return NOT_ERROR;
}

static language_error_t create_local_table_from_str (char* str_with_table, list_of_local_name_tables_t* list_of_local_name_tables, 
                                                     size_t* ptr_index_in_str, size_t quantity_of_id, long scope)
{
	assert (str_with_table);
	assert (list_of_local_name_tables);
	assert (ptr_index_in_str);

	language_error_t status = NOT_ERROR;

	size_t index_id_in_name_table = 0;
	int    type_id                = 0;  
	int    shift_in_str           = 0;

	for (size_t index_id = 0; index_id < quantity_of_id; index_id++)
	{
		sscanf (str_with_table + *ptr_index_in_str, "%ld%n", &index_id_in_name_table, &shift_in_str);
		*ptr_index_in_str += shift_in_str;

		skip_spaces (str_with_table, ptr_index_in_str);
		if (str_with_table[*ptr_index_in_str] == '\0') {return ERROR_IN_CREATE_LOCAL_TABLE_FROM_STR;}

		//------------------------------------------------------------------------------------------------------------------

		sscanf (str_with_table + *ptr_index_in_str, "%d%n", &type_id, &shift_in_str);
		*ptr_index_in_str += shift_in_str;

		skip_spaces (str_with_table, ptr_index_in_str);
		if (str_with_table[*ptr_index_in_str] == '\0' && index_id != quantity_of_id - 1) {return ERROR_IN_CREATE_LOCAL_TABLE_FROM_STR;}

		status = add_new_name_in_local_name_table (list_of_local_name_tables, scope, index_id_in_name_table, (type_id_t) type_id);
		if (status) {return status;}
	}

	return NOT_ERROR;
}