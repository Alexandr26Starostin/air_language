#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "const_language.h"
#include "const_in_front_end.h"
#include "list_of_func.h"
#include "name_table.h"
#include "tokens.h"
#include "lexical_analysis.h"

static language_error_t skip_symbols  (char* str_with_program, size_t* ptr_index_str);
static language_error_t skip_comments (char* str_with_program, size_t* ptr_index_str);

//------------------------------------------------------------------------------------

language_error_t lexical_analysis (array_of_tokens_t* tokens, name_table_t* name_table, list_of_func_t* list_of_func, char* str_with_program)
{
	assert (tokens);
	assert (name_table);
	assert (list_of_func);
	assert (str_with_program);

	language_error_t status                    = NOT_ERROR;
	int               shift_index_str           = 0;
	size_t            index_str                 = 0;
	long              value_of_enum_for_word    = 0;
	size_t            position_id_in_name_table = 0;

	char word_from_str[MAX_LEN_WORD_FROM_STR] = "";

	while (str_with_program[index_str] != '\0')
	{
		skip_symbols (str_with_program, &index_str);
		if (str_with_program[index_str] == '\0') {break;}

		if (('0' <= str_with_program[index_str]     && str_with_program[index_str]     <= '9') || 
		    ('0' <= str_with_program[index_str + 1] && str_with_program[index_str + 1] <= '9' && str_with_program[index_str] == '-'))
		{
			char* copy_str = str_with_program + index_str;

			double number_from_str = strtod (copy_str, &copy_str);

			index_str = copy_str - str_with_program;

			status = add_token_in_array_of_tokens (tokens, NUM, number_from_str);
			if (status) {return status;}

			continue;
		}

		if (str_with_program[index_str] == '\n')
		{
			strcpy (word_from_str, "\n");          //Не эффективно! Можно решить эту проблему, если сделать разделяющим оператором  не '\n', а, например, ';'.
			shift_index_str = 1;
		}
		else
			sscanf (str_with_program + index_str, "%s%n", word_from_str, &shift_index_str);   //%s%n <-> %[^ ]%n

		//printf ("%10s ", word_from_str);

		value_of_enum_for_word = find_word_in_list_of_func (list_of_func, word_from_str);

		//printf ("%4ld    ", value_of_enum_for_word);

		if (value_of_enum_for_word != WORD_IS_NOT_FUNC)
		{
			if (value_of_enum_for_word == (long) COMMENT)
			{
				skip_comments (str_with_program, &index_str);
				continue;
			} 

			status = add_token_in_array_of_tokens (tokens, OP, (double) value_of_enum_for_word);
			if (status) {return status;}

			index_str += shift_index_str;

			//printf ("%ld\n", index_str);
			continue;
		}

		position_id_in_name_table = find_position_name (name_table, index_str, shift_index_str, str_with_program);

		//printf ("%4ld\n", position_id_in_name_table);

		if (position_id_in_name_table == name_table -> free_index)
		{
			status = add_name_in_table (name_table, index_str, shift_index_str, str_with_program);
			if (status) {return status;}
		}

		status = add_token_in_array_of_tokens (tokens, ID, (double) position_id_in_name_table);
		if (status) {return status;}

		index_str += shift_index_str;
	}

	return NOT_ERROR;
}

static language_error_t skip_symbols (char* str_with_program, size_t* ptr_index_str)
{
	assert (str_with_program);
	assert (ptr_index_str);

	while (str_with_program[*ptr_index_str] == ' '  || 
		   str_with_program[*ptr_index_str] == '\t')
	{
		*ptr_index_str += 1;
	}

	return NOT_ERROR;
}

static language_error_t skip_comments (char* str_with_program, size_t* ptr_index_str)
{
	assert (str_with_program);
	assert (ptr_index_str);

	while (str_with_program[*ptr_index_str] != '\n') {*ptr_index_str += 1;}

	*ptr_index_str += 1;

	return NOT_ERROR;
}