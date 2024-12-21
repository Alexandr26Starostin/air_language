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

language_error_t launch_reverse_front_end (int argc, char** argv)
{
	assert (argv);

	//-------------------------------------------------------------------------------------------------------
	/*read file with tree and create str_with_tree*/

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END

		printf ("\n\n---------------------------------------------------------------------------\n\nstr_with_tree:\n\n");

	#endif

	char* str_with_tree = NULL;

	language_error_t status = read_file_to_str (argc, argv, &str_with_tree, FIND_FILE_WITH_TREE, NOT_FIND_FILE_WITH_TREE);
	if (status) {return status;}

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END

		printf ("%s\n\n", str_with_tree);

	#endif

	//-------------------------------------------------------------------------------------------------------
	/*read file with table and create str_with_table*/

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END

		printf ("\n\n---------------------------------------------------------------------------\n\nstr_with_table:\n\n");
	
	#endif

	char* str_with_table = NULL;

	status = read_file_to_str (argc, argv, &str_with_table, FIND_FILE_WITH_TABLE, NOT_FIND_FILE_WITH_TABLE);
	if (status) {return status;}

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END

		printf ("%s\n\n", str_with_table);
	
	#endif

	//-------------------------------------------------------------------------------------------------------
	/*create list_of_func*/

	list_of_func_t list_of_func = {};

	status = create_list_of_func (&list_of_func);
	if (status) {return status;}

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END
	
		dump_list_of_func (&list_of_func);

	#endif

	//-------------------------------------------------------------------------------------------------------
	/*create tree*/

	node_t* root_node              = NULL;
	size_t  index_in_str_with_tree = 0;

	status = create_tree_from_str (str_with_tree, &root_node, &index_in_str_with_tree);
	if (status) {return status;}

	//-------------------------------------------------------------------------
	/*create name_table*/

	name_table_t name_table = {};

	status = create_name_table (&name_table);
	if (status) {return status;}

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END

		dump_name_table (&name_table, str_with_table);
	
	#endif

	size_t index_in_str_with_table = 0;

	status = create_table_from_str (str_with_table, &name_table, &index_in_str_with_table);
	if (status) {return status;}

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END

		dump_name_table (&name_table, str_with_table);

	#endif

	//-------------------------------------------------------------------------------------------------------
	/*dump tree*/

	char str_for_system[] = "dot tree.dot -Tsvg -o pictures/diff_0000.svg"; 

	size_t index_picture = 0;

	FILE* tree_html = fopen ("tree.html", "w");
	if (tree_html == NULL) {printf ("Not find tree.html\n"); return NOT_FIND_TREE_HTML;}

	dump_tree_t tree_dump = {};

	tree_dump.index_picture    = index_picture;
	tree_dump.list_of_func     = &list_of_func;
	tree_dump.name_table       = &name_table;
	tree_dump.str_for_system   = str_for_system;
	tree_dump.str_with_program = str_with_table;
	tree_dump.tree_html        = tree_html;

	#ifdef CHECK_WORK_OF_REVERSE_FRONT_END

		dump_tree (root_node, &tree_dump);

	#endif

	//-------------------------------------------------------------------------------------------------------
	/*write program from tree*/

	status = write_program_from_tree (argc, argv, root_node, str_with_table, &name_table, &list_of_func);
	if (status) {return status;}

	//-------------------------------------------------------------------------------------------------------
	/*free memory*/

	delete_node         			 (root_node);
	delete_list_of_func 			 (&list_of_func);
	delete_name_table   			 (&name_table);
	// delete_list_of_local_name_tables (&list_of_local_name_tables);

	free (str_with_tree);
	free (str_with_table);

	return NOT_ERROR;
}