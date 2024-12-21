#ifndef CONST_IN_REVERSE_FRONT_END_H
#define CONST_IN_REVERSE_FRONT_END_H


//--------------------------------------------------------------------------------------
/*write_program_from_tree*/

struct reverse_t
{
	char*           str_with_table;
	name_table_t*   name_table;
	size_t          depth_of_tabulation;
	FILE*           reverse_file; 
	list_of_func_t* list_of_func;
};

#endif