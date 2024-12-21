#ifndef CONST_IN_BACK_END_H
#define CONST_IN_BACK_END_H

//---------------------------------------------------
/*write_tree_in_asm*/

enum position_in_assign_t
{
	NOT_IN_ASSIGN = 0,
	IN_ASSIGN     = 1
};

enum call_func_t 
{
	MAIN_FUNC = 0,
	CALL_FUNC = 1
};

struct tree_in_asm_t
{
	FILE*                        file_for_asm;
	name_table_t*                name_table;
	list_of_local_name_tables_t* list_of_local_name_tables;
	position_in_assign_t         position_in_assign;
	size_t                       index_of_if;
	size_t                       index_of_while;
	size_t                       index_of_operator;
	char*                        str_with_table;
	size_t                       depth_of_tabulation;
	long                         scope;
	call_func_t                  status_of_func;
};

#endif