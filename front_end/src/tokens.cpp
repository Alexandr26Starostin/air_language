#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "const_in_front_end.h"
#include "name_table.h"
#include "tokens.h"

static front_end_error_t realloc_array_of_tokens (array_of_tokens_t* tokens);

//----------------------------------------------------------------------------------------------------

front_end_error_t create_tokens (array_of_tokens_t* tokens, name_t* name_table)
{
	assert (tokens);
	assert (name_table);

	tokens -> array_of_tokens = (token_t*) calloc (SIZE_ARRAY_OF_TOKENS, sizeof (token_t));

	if (tokens -> array_of_tokens == NULL)
	{
		printf ("\nError in %s:%d\nNot memory for array_of_tokens\n\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_ARRAY_OF_TOKENS;
	}

	tokens -> size_of_array = SIZE_ARRAY_OF_TOKENS;
	tokens -> index_free    = 0;
	tokens -> name_table    = name_table;

	return NOT_ERROR;
}

front_end_error_t delete_tokens (array_of_tokens_t* tokens)
{
	assert (tokens);
	
	free (tokens -> array_of_tokens);

	tokens -> size_of_array = 0;
	tokens -> index_free    = 0;
	tokens -> name_table    = NULL;

	return NOT_ERROR;
}

front_end_error_t add_token_in_array_of_tokens (array_of_tokens_t* tokens, token_type_t type, double value)
{
	assert (tokens);

	(tokens -> array_of_tokens)[tokens -> index_free].type = type;

	switch (type)
	{
		case NUM: 
			(tokens -> array_of_tokens)[tokens -> index_free].value.value_num = value;
			break;

		case OP: 
			(tokens -> array_of_tokens)[tokens -> index_free].value.value_op = (base_func_t) value;
			break;

		case ID: 
		{ 	
			(tokens -> array_of_tokens)[tokens -> index_free].value.value_id = (size_t) value;

			/*Это индекс на имя ID в name_table. 
			Значение индекса находится перед вызовом функции 'add_token_in_array_of_tokens' с помощью функции 'find_position_name'*/

			break;
		}

		default: break;
	}

	tokens -> index_free += 1;

	if (tokens -> index_free >= tokens -> size_of_array)
	{
		front_end_error_t status = realloc_array_of_tokens (tokens);
		if (status) {return status;}
	}

	return NOT_ERROR;
}

static front_end_error_t realloc_array_of_tokens (array_of_tokens_t* tokens)
{
	assert (tokens);

	tokens -> size_of_array *= 2;

	tokens -> array_of_tokens = (token_t*) realloc (tokens -> array_of_tokens, sizeof (token_t) * tokens -> size_of_array);

	if (tokens -> array_of_tokens == NULL)
	{
		printf ("\nError in %s:%d\nNot memory for realloc of array_of_tokens\n\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_REALLOC_ARRAY_OF_TOKENS;
	}

	return NOT_ERROR;
}

front_end_error_t dump_array_of_tokens (array_of_tokens_t* tokens)
{
	assert (tokens);

	printf ("\n\n------------------------------------------------------------------\n\n");
	printf ("array_of_tokens:\n\nsize_of_array == %ld\nfree_index    == %ld\n\n", tokens -> size_of_array, tokens -> index_free);

	printf ("list_of_type:\nNUM == 1\nOP  == 2\nID  == 3\n\n");

	printf ("index    type    value\n");

	for (size_t index = 0; index < tokens -> index_free; index++)
	{
		
		printf ("%5ld    %4d    ", index, (tokens -> array_of_tokens)[index].type);

		switch ((tokens -> array_of_tokens)[index].type)
		{
			case NUM: 
				printf ("%5lg\n", (tokens -> array_of_tokens)[index].value.value_num);
				break;

			case OP: 
				printf ("%5d\n", (tokens -> array_of_tokens)[index].value.value_op);
				break;

			case ID:
				printf ("%5ld\n", (tokens -> array_of_tokens)[index].value.value_id);
				break;
			
			default: break;
		}
	}

	printf ("\n\n------------------------------------------------------------------\n\n");

	return NOT_ERROR;
}