#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "const_in_front_end.h"
#include "local_name_table.h"

static front_end_error_t create_local_name_table           (local_name_table_t* local_name_table, long scope_of_local_name_table);
static front_end_error_t realloc_list_of_local_name_tables (list_of_local_name_tables_t* list_of_local_name_tables);
static front_end_error_t realloc_local_name_table          (local_name_table_t* local_name_table);
static front_end_error_t delete_local_name_table           (local_name_table_t* local_name_table);
static front_end_error_t dump_local_name_table             (local_name_table_t* local_name_table);

//------------------------------------------------------------------------------------------------------------------------------------------

front_end_error_t create_list_of_local_name_tables (list_of_local_name_tables_t* list_of_local_name_tables)
{
	assert (list_of_local_name_tables);

	list_of_local_name_tables -> array_of_local_name_table = (local_name_table_t*) calloc (MIN_SIZE_ARRAY_OF_LOCAL_NAME_TABLE, sizeof (local_name_table_t));

	if (list_of_local_name_tables -> array_of_local_name_table == NULL)
	{
		printf ("Error in %s:%d\nNot memory for array_of_local_name_table\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_ARRAY_OF_LOCAL_NAME_TABLE;
	}

	list_of_local_name_tables -> size_list          = MIN_SIZE_ARRAY_OF_LOCAL_NAME_TABLE;
	list_of_local_name_tables -> free_index_in_list = 0;

	return NOT_ERROR;
}

front_end_error_t create_local_name_table (local_name_table_t* local_name_table, long scope_of_local_name_table)
{
	assert (local_name_table);

	local_name_table -> array_of_local_names = (local_name_t*) calloc (MIN_SIZE_ARRAY_OF_LOCAL_NAMES, sizeof (local_name_t));

	if (local_name_table -> array_of_local_names == NULL)
	{
		printf ("Error in %s:%d\nNot memory for array_of_local_names\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_ARRAY_OF_LOCAL_NAMES;
	}

	local_name_table -> size_local_name_table          = MIN_SIZE_ARRAY_OF_LOCAL_NAMES;
	local_name_table -> free_index_in_local_name_table = 0;
	local_name_table -> scope_of_local_name_table      = scope_of_local_name_table;

	return NOT_ERROR;
}

front_end_error_t add_new_local_name_table_in_list (list_of_local_name_tables_t* list_of_local_name_tables, long scope_of_local_name_table)
{
	assert (list_of_local_name_tables);

	//printf ("%p\n", &((list_of_local_name_tables -> array_of_local_name_table)[list_of_local_name_tables -> free_index_in_list]));

	front_end_error_t status = create_local_name_table (&((list_of_local_name_tables -> array_of_local_name_table)[list_of_local_name_tables -> free_index_in_list]), 
	                                                    scope_of_local_name_table);
													
	if (status) {return status;}

	list_of_local_name_tables -> free_index_in_list += 1;

	if (list_of_local_name_tables -> free_index_in_list >= list_of_local_name_tables -> size_list)
	{
		status = realloc_list_of_local_name_tables (list_of_local_name_tables);
		if (status) {return status;}
	}

	return NOT_ERROR;
}

front_end_error_t realloc_list_of_local_name_tables (list_of_local_name_tables_t* list_of_local_name_tables)
{
	assert (list_of_local_name_tables);

	size_t new_size_list = 2 * list_of_local_name_tables -> size_list;

	list_of_local_name_tables -> array_of_local_name_table = (local_name_table_t*) realloc (list_of_local_name_tables -> array_of_local_name_table, new_size_list * sizeof (local_name_table_t));

	if (list_of_local_name_tables -> array_of_local_name_table == NULL)
	{
		printf ("Error in %s:%d\nNot memory for realloc array_of_local_name_table\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_REALLOC_ARRAY_OF_LOCAL_NAME_TABLE;
	}

	list_of_local_name_tables -> size_list = new_size_list;

	return NOT_ERROR;
}

front_end_error_t add_new_name_in_local_name_table (list_of_local_name_tables_t* list_of_local_name_tables, long scope_of_local_name_table, size_t index_id_in_name_table, type_id_t type_id)
{
	assert (list_of_local_name_tables);

	for (size_t index_in_list = 0; index_in_list < list_of_local_name_tables -> free_index_in_list; index_in_list++)
	{
		local_name_table_t* find_local_name_table = &((list_of_local_name_tables -> array_of_local_name_table)[index_in_list]);

		if (find_local_name_table -> scope_of_local_name_table != scope_of_local_name_table)
			continue;

		(find_local_name_table -> array_of_local_names)[find_local_name_table -> free_index_in_local_name_table].index_id_in_name_table = index_id_in_name_table;
		(find_local_name_table -> array_of_local_names)[find_local_name_table -> free_index_in_local_name_table].type_id                = type_id;

		find_local_name_table -> free_index_in_local_name_table += 1;

		if (! (find_local_name_table -> free_index_in_local_name_table >= find_local_name_table -> size_local_name_table))
		{
			return NOT_ERROR;
		}

		return realloc_local_name_table (&((list_of_local_name_tables -> array_of_local_name_table)[index_in_list]));
	}

	printf ("Error in %s:%d\nNot find local_name_table with scope == %ld", __FILE__, __LINE__, scope_of_local_name_table);

	return NOT_FIND_LOCAL_NAME_TABLE_WITH_SCOPE;
}

front_end_error_t realloc_local_name_table (local_name_table_t* local_name_table)
{
	assert (local_name_table);

	size_t new_size = 2 * local_name_table -> size_local_name_table;

	local_name_table -> array_of_local_names = (local_name_t*) realloc (local_name_table -> array_of_local_names, new_size * sizeof (local_name_t));

	if (local_name_table -> array_of_local_names == NULL)
	{
		printf ("Error in %s:%d\nNot memory for realloc array_of_local_names\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_REALLOC_ARRAY_OF_LOCAL_NAMES;
	}

	local_name_table -> size_local_name_table = new_size;

	return NOT_ERROR;
}

front_end_error_t delete_list_of_local_name_tables (list_of_local_name_tables_t* list_of_local_name_tables)
{
	assert (list_of_local_name_tables);

	for (size_t index_in_list = 0; index_in_list < list_of_local_name_tables -> free_index_in_list; index_in_list++)
	{
		delete_local_name_table (&((list_of_local_name_tables -> array_of_local_name_table)[index_in_list]));
	}

	free (list_of_local_name_tables -> array_of_local_name_table);

	list_of_local_name_tables -> free_index_in_list = 0;
	list_of_local_name_tables -> size_list          = 0;

	return NOT_ERROR;
}

front_end_error_t delete_local_name_table (local_name_table_t* local_name_table)
{
	assert (local_name_table);

	free (local_name_table -> array_of_local_names);

	local_name_table -> free_index_in_local_name_table = 0;
	local_name_table -> size_local_name_table          = 0;
	local_name_table -> scope_of_local_name_table      = NOT_DEFINITE_SCOPE;

	return NOT_ERROR;
}

front_end_error_t dump_list_of_local_name_tables (list_of_local_name_tables_t* list_of_local_name_tables)
{
	assert (list_of_local_name_tables);

	printf ("\n\n---------------------------------------------------------------------------------------\n\n");

	printf ("list_of_local_name_tables:\n\n");

	printf ("size_list          == %ld\n", list_of_local_name_tables -> size_list);
	printf ("free_index_in_list == %ld\n\n", list_of_local_name_tables -> free_index_in_list);

	for (size_t index_list = 0; index_list < list_of_local_name_tables -> free_index_in_list; index_list++)
	{
		dump_local_name_table (&((list_of_local_name_tables -> array_of_local_name_table)[index_list]));
	}

	printf ("\n\n---------------------------------------------------------------------------------------\n\n");

	return NOT_ERROR;
}

front_end_error_t dump_local_name_table (local_name_table_t* local_name_table)
{
	assert (local_name_table);

	printf ("\tscope_of_local_name_table     == %ld\n", local_name_table -> scope_of_local_name_table);
	printf ("\tsize_local_name_table         == %ld\n", local_name_table -> size_local_name_table);
	printf ("\tree_index_in_local_name_table == %ld\n", local_name_table -> free_index_in_local_name_table);
	printf ("\tindex_id_in_name_table    type_id\n");

	for (size_t index = 0; index < local_name_table -> free_index_in_local_name_table; index++)
	{
		printf ("\t%22ld    %7d\n", (local_name_table -> array_of_local_names)[index].index_id_in_name_table,
		                                                 (local_name_table -> array_of_local_names)[index].type_id);
	}

	printf ("\n\n");

	return NOT_ERROR;
}

size_t find_position_of_local_table (list_of_local_name_tables_t* list_of_local_name_tables, long scope_of_local_name_table)
{
	assert (list_of_local_name_tables);

	size_t index_in_list = 0;
	
	for (; index_in_list < list_of_local_name_tables -> free_index_in_list; index_in_list++)
	{
		if ((list_of_local_name_tables -> array_of_local_name_table)[index_in_list].scope_of_local_name_table == scope_of_local_name_table)
		{
			break;
		}
	}

	return index_in_list;
}