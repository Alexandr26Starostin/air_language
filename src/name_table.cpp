#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "const_language.h"
#include "name_table.h"

static language_error_t realloc_name_table (name_table_t* name_table);
static size_t            max_len            (size_t len_1, size_t len_2);

//-------------------------------------------------------------------------------------------------------------------------------------------

language_error_t create_name_table (name_table_t* name_table)
{
	assert (name_table);

	name_table -> array_names = (name_t*) calloc (SIZE_NAME_TABLE, sizeof (name_t));

	if (name_table -> array_names == NULL)
	{
		printf ("\nError in %s:%d\nNot memory for name_table\n\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_NAME_TABLE;
	}

	name_table -> size_of_name_table = SIZE_NAME_TABLE;
	name_table -> free_index         = 0;

	return NOT_ERROR;
}

language_error_t delete_name_table (name_table_t* name_table)
{
	assert (name_table);

	free (name_table -> array_names);

	name_table -> size_of_name_table = 0;
	name_table -> free_index         = 0;

	return NOT_ERROR;
}

language_error_t add_name_in_table (name_table_t* name_table, size_t index_to_name_in_str, size_t len_name, char* str_with_program)
{
	assert (name_table);
	assert (str_with_program);

	size_t position_name_in_table = find_position_name (name_table, index_to_name_in_str, len_name, str_with_program);

	if (position_name_in_table == name_table -> free_index)
	{
		(name_table -> array_names)[name_table -> free_index].index_to_name_in_str = index_to_name_in_str;
		(name_table -> array_names)[name_table -> free_index].len_name             = len_name;
		(name_table -> array_names)[name_table -> free_index].type                 = UNKNOW_TYPE;
		(name_table -> array_names)[name_table -> free_index].status               = NOT_DEFINITE;

		name_table -> free_index += 1;

		if (name_table -> free_index >= name_table -> size_of_name_table)
		{
			language_error_t status = realloc_name_table (name_table);
			if (status) {return status;}
		}
	}

	return NOT_ERROR;
}

static language_error_t realloc_name_table (name_table_t* name_table)
{
	assert (name_table);

	name_table -> size_of_name_table *= 2;

	name_table -> array_names = (name_t*) realloc (name_table -> array_names, sizeof (name_t) * name_table -> size_of_name_table);

	if (name_table -> array_names == NULL)
	{
		printf ("\nError in %s:%d\nNot memory for realloc of name_table\n\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_REALLOC_NAME_TABLE;
	}

	return NOT_ERROR;
}

size_t find_position_name (name_table_t* name_table, size_t find_index, size_t len_name, char* str_with_program)
{
	assert (name_table);
	assert (str_with_program);

	size_t index = 0;

	for (; index < name_table -> free_index; index++)
	{
		if (strncmp  (str_with_program + (name_table -> array_names)[index].index_to_name_in_str, str_with_program + find_index, 
		                        max_len ((name_table -> array_names)[index].len_name, len_name)) == 0)
		{
			return index ;
		}
	}

	return index;   //index == name_table -> free_index
}

language_error_t dump_name_table   (name_table_t* name_table, char* str_with_program)
{
	assert (name_table); 

	printf ("\n\n------------------------------------------------------------------\n\n");
	printf ("name_table:\n\nsize_of_name_table == %ld\nfree_index         == %ld\n\n", name_table-> size_of_name_table, name_table-> free_index);

	printf ("type:\nUNKNOW_TYPE == 0\nNAME_VAR    == 1\nNAME_FUNC   == 2\nNAME_TYPE   == 4\n\n");
	
	printf ("status_of_declaration:\nNOT_DEFINITE == 0\nDEFINITE     == 1\n\n");

	if (str_with_program == NULL)
	{
		printf ("index    index_to_name_in_str    len_name    type    status_of_declaration\n");

		for (size_t index = 0; index < name_table -> free_index; index++)
		{	
			printf ("%5ld    %20ld    %8ld    %4d    %21d\n", index, (name_table -> array_names)[index].index_to_name_in_str, 
																	 (name_table -> array_names)[index].len_name, 
																	 (name_table -> array_names)[index].type,
																	 (name_table -> array_names)[index].status);
		}
	}

	else   //str_with_program != NULL
	{
		printf ("index    index_to_name_in_str    len_name    type    status_of_declaration    name_of_id\n");

		for (size_t index = 0; index < name_table -> free_index; index++)
		{
			
			printf ("%5ld    %20ld    %8ld    %4d    %21d    ", index, (name_table -> array_names)[index].index_to_name_in_str, 
																	   (name_table -> array_names)[index].len_name, 
																	   (name_table -> array_names)[index].type, 
																	   (name_table -> array_names)[index].status);

			print_symbols_from_str (str_with_program + (name_table -> array_names)[index].index_to_name_in_str, 
			                                           (name_table -> array_names)[index].len_name);
		}
	}

	printf ("\n\n------------------------------------------------------------------\n\n");

	return NOT_ERROR;
}

language_error_t print_symbols_from_str (char* str, size_t count_of_symbols)
{
	assert (str);

	for (size_t index = 0; index < count_of_symbols; index++)
	{
		printf ("%c", str[index]);
	}
	printf ("\n");

	return NOT_ERROR;
}

static size_t max_len (size_t len_1, size_t len_2)
{
	if (len_1 > len_2) {return len_1;}

	return len_2;
}

language_error_t print_symbols_from_str_in_file (FILE* file, char* str, size_t count_of_symbols)
{
	assert (str);
	assert (file);

	for (size_t index = 0; index < count_of_symbols; index++)
	{
		fprintf (file, "%c", str[index]);
	}
	
	return NOT_ERROR;
}