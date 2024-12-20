#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "const_language.h"
#include "const_in_back_end.h"
#include "operations_with_files.h"
#include "tree.h"
//#include "list_of_func.h"
#include "name_table.h"
#include "local_name_table.h"
#include "write_tree_in_asm.h"

static language_error_t write_node_in_asm (FILE* file_for_asm, node_t* node, name_table_t* name_table, list_of_local_name_tables_t* list_of_local_name_tables);

//------------------------------------------------------------------------------------------------------------------------------------

language_error_t write_tree_in_asm (int argc, char** argv, node_t* root_node, name_table_t* name_table, list_of_local_name_tables_t* list_of_local_name_tables)
{
	assert (argv);
	assert (root_node);
	assert (name_table);
	assert (list_of_local_name_tables);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = find_program_file (argc, argv, FIND_FILE_FOR_ASM);
	if (file_for_asm == NULL) {return NOT_FIND_FILE_FOR_ASM;}

	status = write_node_in_asm (file_for_asm, root_node, name_table, list_of_local_name_tables);
	if (status) {return status;}

	fclose (file_for_asm);

	return NOT_ERROR;
}

static language_error_t write_node_in_asm (FILE* file_for_asm, node_t* node, name_table_t* name_table, list_of_local_name_tables_t* list_of_local_name_tables)
{
	assert (file_for_asm);
	assert (name_table);
	assert (list_of_local_name_tables);

	if (node == NULL) {return NOT_ERROR;}

	return NOT_ERROR;
}