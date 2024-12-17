#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "const_in_front_end.h"
#include "list_of_func.h"
#include "operations_with_files.h"
#include "name_table.h"
#include "tokens.h"
#include "lexical_analysis.h"
#include "tree.h"
#include "recursive_descent.h"
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

	//-------------------------------------------------------------------------
	/*create tokens*/

	array_of_tokens_t tokens = {};

	status = create_tokens (&tokens, name_table.name_table);
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
	if (root_node == NULL) return NOT_MEMORY_FOR_NEW_NODE;\

	FILE* tree_html = fopen ("tree.html", "w");
	if (tree_html == NULL) {printf ("Not find tree.html\n"); return NOT_FIND_TREE_HTML;}

	dump_tree (root_node, str_for_system, &index_picture, tree_html);

	//------------------------------------------------------------------------
	/*recursive descent*/

	status = recursive_descent (&tokens, root_node);
	if (status) {return status;}

	dump_tree (root_node, str_for_system, &index_picture, tree_html);

	//------------------------------------------------------------------------
	/*write AST, name_table and local name_tables in files*/

	//-------------------------------------------------------------------------
	/*free memory*/

	delete_tokens 	    (&tokens);
	delete_name_table   (&name_table);
	delete_list_of_func (&list_of_func);
	delete_node         (root_node);

	free (str_with_program);

	return status;
}

//доделать освобождение памяти при срабатывании ошибок через status.
