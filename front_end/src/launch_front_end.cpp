#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "const_in_front_end.h"
#include "list_of_func.h"
#include "operations_with_files.h"
#include "name_table.h"
#include "tokens.h"
#include "lexical_analysis.h"
#include "tree.h"
#include "recursive_descent.h"
#include "local_name_table.h"
#include "launch_front_end.h"

front_end_error_t launch_front_end (int argc, char** argv)
{
	assert (argv);

	//-----------------------------------------------------------------------
	/*create list_of_func*/

	list_of_func_t list_of_func = {};

	front_end_error_t status = create_list_of_func (&list_of_func);
	if (status) {return status;}
	
	dump_list_of_func (&list_of_func);

	//------------------------------------------------------------------------
	/*read program file and create str_with_program*/

	char* str_with_program = NULL;

	status = read_program_file (argc, argv, &str_with_program);
	if (status) {return status;}

	printf ("%s\n", str_with_program);

	//-------------------------------------------------------------------------
	/*create name_table*/

	name_table_t name_table = {};

	status = create_name_table (&name_table);
	if (status) {return status;}

	dump_name_table (&name_table, str_with_program);

	printf ("len_str_with_program == %ld\n\n", strlen (str_with_program));

	//-------------------------------------------------------------------------
	/*create tokens*/

	array_of_tokens_t tokens = {};

	status = create_tokens (&tokens);
	if (status) {return status;}

	dump_array_of_tokens (&tokens);

	//------------------------------------------------------------------------
	/*lexical_analysis*/

	status = lexical_analysis (&tokens, &name_table, &list_of_func, str_with_program);
	if (status) {return status;}

	printf ("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nlexical_analysis completed\n\n");

	dump_name_table (&name_table, str_with_program);
	dump_array_of_tokens (&tokens);

	//------------------------------------------------------------------------
	/*create tree*/

	char str_for_system[] = "dot tree.dot -Tsvg -o pictures/diff_0000.svg"; 

	size_t index_picture = 0;

	//node_t* root_node = NULL;

	node_t* root_node = create_node (KEYWORD, OPERATOR, NULL, NULL, NULL);
	if (root_node == NULL) return NOT_MEMORY_FOR_NEW_NODE;

	FILE* tree_html = fopen ("tree.html", "w");
	if (tree_html == NULL) {printf ("Not find tree.html\n"); return NOT_FIND_TREE_HTML;}

	dump_tree_t tree_dump = {};

	tree_dump.index_picture    = index_picture;
	tree_dump.list_of_func     = &list_of_func;
	tree_dump.name_table       = &name_table;
	tree_dump.str_for_system   = str_for_system;
	tree_dump.str_with_program = str_with_program;
	tree_dump.tokens           = &tokens;
	tree_dump.tree_html        = tree_html;

	dump_tree (root_node, &tree_dump);

	//------------------------------------------------------------------------
	/*list_of_local_name_tables*/

	list_of_local_name_tables_t list_of_local_name_tables = {};

	status = create_list_of_local_name_tables (&list_of_local_name_tables);
	if (status) {return status;}

	status = add_new_local_name_table_in_list  (&list_of_local_name_tables, GLOBAL_SCOPE);
	if (status) {return status;}

	dump_list_of_local_name_tables (&list_of_local_name_tables);

	//------------------------------------------------------------------------
	/*recursive descent*/

	status = recursive_descent (&tokens, &name_table, &list_of_local_name_tables, root_node);
	if (status) {return status;}

	printf ("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nrecursive_descent\n\n");

	dump_name_table (&name_table, str_with_program);

	dump_list_of_local_name_tables (&list_of_local_name_tables);

	dump_tree (root_node, &tree_dump);

	//------------------------------------------------------------------------
	/*write AST, name_table and local name_tables in files*/

	status = write_tree_in_file (argc, argv, root_node);
	if (status) {return status;}

	status = write_table_in_file (argc, argv, &list_of_local_name_tables, &name_table, str_with_program);
	if (status) {return status;}

	//-------------------------------------------------------------------------
	/*free memory*/

	delete_tokens 	                 (&tokens);
	delete_name_table                (&name_table);
	delete_list_of_func              (&list_of_func);
	delete_node                      (root_node);
	delete_list_of_local_name_tables (&list_of_local_name_tables);

	free (str_with_program);

	return status;
}

//доделать освобождение памяти при срабатывании ошибок через status.
