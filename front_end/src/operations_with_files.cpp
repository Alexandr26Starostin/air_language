#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "const_in_front_end.h"
#include "name_table.h"
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

static size_t            count_len_file                          (FILE* file);
static front_end_error_t print_node_in_file                      (FILE* file_for_tree, node_t* node);
static front_end_error_t print_name_table_in_file                (FILE* file_for_table, name_table_t* name_table, char* str_with_program);
static front_end_error_t print_list_of_local_name_tables_in_file (FILE* file_for_table, list_of_local_name_tables_t* list_of_local_name_tables);
static front_end_error_t print_local_name_tables_in_file         (FILE* file_for_table, local_name_table_t* local_name_table);

//-----------------------------------------------------------------------------------------------------------------------------------------

front_end_error_t read_program_file (int argc, char** argv, char** ptr_str_with_program)
{
	assert (argv);
	assert (ptr_str_with_program);

	FILE* program_file = find_program_file (argc, argv, FIND_FILE_WITH_PROGRAM);
	if (program_file == NULL) {return NOT_FIND_FILE_WITH_PROGRAM;}

	size_t len_file = count_len_file (program_file);

	*ptr_str_with_program = (char*) calloc (len_file + 1, sizeof (char));
	if (*ptr_str_with_program == NULL)
	{
		printf ("\n\nError in %s:%d\n"
		        "not memory for str_with_program\n\n", __FILE__, __LINE__);

		fclose (program_file);

		return NOT_MEMORY_FOR_STR_WITH_PROGRAM;
	}

	fread (*ptr_str_with_program, sizeof (char), len_file, program_file);

	fclose (program_file);

	return NOT_ERROR;
}

FILE* find_program_file (int argc, char** argv, operation_with_file_t operation_with_file)
{
	assert (argv);

	char* name_file = NULL;

	switch (operation_with_file)
	{
		FIND_FILE_(FIND_FILE_WITH_PROGRAM, "-read_program", "r");
		FIND_FILE_(FIND_FILE_WITH_TREE,    "-read_tree",    "r");
		FIND_FILE_(FIND_FILE_WITH_TABLE,   "-read_table",   "r");
		FIND_FILE_(FIND_FILE_FOR_TREE,     "-write_tree",   "w");
		FIND_FILE_(FIND_FILE_FOR_TABLE,    "-write_table",  "w");
		
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

front_end_error_t write_tree_in_file (int argc, char** argv, node_t* root_node)
{
	assert (argv);
	assert (root_node);

	FILE* file_for_tree = find_program_file (argc, argv, FIND_FILE_FOR_TREE);
	if (file_for_tree == NULL) {return NOT_FIND_FILE_FOR_TREE;}

	print_node_in_file (file_for_tree, root_node);

	fclose (file_for_tree);

	return NOT_ERROR;
}

static front_end_error_t print_node_in_file (FILE* file_for_tree, node_t* node)
{
	assert (file_for_tree);

	if (node != NULL && node -> type == KEYWORD && (node -> value).value_keyword == OPERATOR && node -> left == NULL)
	{
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

front_end_error_t write_table_in_file (int argc, char** argv, list_of_local_name_tables_t* list_of_local_name_tables, name_table_t* name_table, char* str_with_program)
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


static front_end_error_t print_name_table_in_file (FILE* file_for_table, name_table_t* name_table, char* str_with_program)
{
	assert (file_for_table);
	assert (name_table);
	assert (str_with_program);

	fprintf (file_for_table, "%ld\n", name_table -> free_index);

	for (size_t index = 0; index < name_table -> free_index; index++)
	{
		print_symbols_from_str_in_file (file_for_table, str_with_program + (name_table -> array_names)[index].index_to_name_in_str, 
			                                                               (name_table -> array_names)[index].len_name);
	}

	fprintf (file_for_table, "\n");

	return NOT_ERROR;
}

static front_end_error_t print_list_of_local_name_tables_in_file (FILE* file_for_table, list_of_local_name_tables_t* list_of_local_name_tables)
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

static front_end_error_t print_local_name_tables_in_file (FILE* file_for_table, local_name_table_t* local_name_table)
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