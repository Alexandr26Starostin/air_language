#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "const_in_front_end.h"
#include "list_of_func.h"

#define FILL_LIST_OF_FUNC_(value_code_func, str_with_name_func)                                         \
	case value_code_func:                                                                               \
	{                                                                                                   \
		(list_of_func -> array_of_func)[list_of_func -> free_index].code_func = value_code_func;        \
		(list_of_func -> array_of_func)[list_of_func -> free_index].name_func = str_with_name_func;     \
		break;                                                                                          \
	} 

static front_end_error_t fill_list_of_func    (list_of_func_t* list_of_func);
static front_end_error_t realloc_list_of_func (list_of_func_t* list_of_func);

//---------------------------------------------------------------------------------------------------------------------------------

front_end_error_t create_list_of_func (list_of_func_t* list_of_func)
{
	assert (list_of_func);

	list_of_func -> array_of_func = (name_base_func_t*) calloc (SIZE_LIST_OF_FUNC, sizeof (name_base_func_t));

	if (list_of_func -> array_of_func == NULL)
	{
		printf ("\nError in %s:%d\nNot memory for list_of_func\n\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_LIST_OF_FUNC;
	}

	list_of_func -> free_index   = 0;
	list_of_func -> size_of_list = SIZE_LIST_OF_FUNC;

	front_end_error_t status = fill_list_of_func (list_of_func);

	return status;
}

front_end_error_t delete_list_of_func (list_of_func_t* list_of_func)
{
	assert (list_of_func);

	list_of_func -> free_index   = 0;
	list_of_func -> size_of_list = 0;

	free (list_of_func -> array_of_func);

	return NOT_ERROR;
}

static front_end_error_t fill_list_of_func (list_of_func_t* list_of_func)
{
	assert (list_of_func);

	front_end_error_t status = NOT_ERROR;

	size_t value_of_enum = 0;

	while (value_of_enum <= MAX_VALUE_OF_ENUM)
	{
		switch (value_of_enum)
		{
			FILL_LIST_OF_FUNC_(ADD,         "дозаправить");
			FILL_LIST_OF_FUNC_(SUB,         "ударить");
			FILL_LIST_OF_FUNC_(MUL,         "штопор");
			FILL_LIST_OF_FUNC_(DIV,         "кобра");
			FILL_LIST_OF_FUNC_(SIN,         "ангар");
			FILL_LIST_OF_FUNC_(COS,         "перрон");
			FILL_LIST_OF_FUNC_(SH,          "гидрашка");
			FILL_LIST_OF_FUNC_(CH,          "винт");
			FILL_LIST_OF_FUNC_(SQRT,        "петля");
			FILL_LIST_OF_FUNC_(DEG,         "тяга");
			FILL_LIST_OF_FUNC_(LOG,         "касалетка");
			FILL_LIST_OF_FUNC_(LN,          "иллюминатор");
			FILL_LIST_OF_FUNC_(IF,          "рёв_турбин");
			FILL_LIST_OF_FUNC_(WHILE,       "уносится_в_небо_машина");
			FILL_LIST_OF_FUNC_(MAIN,        "мой_папа_чинит_самолёты");
			FILL_LIST_OF_FUNC_(INT,         "в");
			FILL_LIST_OF_FUNC_(ASSIGN,      "ИЛ");
			FILL_LIST_OF_FUNC_(SCANF,       "осмотреть");
			FILL_LIST_OF_FUNC_(PRINTF,      "починить");
			FILL_LIST_OF_FUNC_(OPERATOR,    "\n");
			FILL_LIST_OF_FUNC_(RETURN,      "как_хочется_в_небо");
			FILL_LIST_OF_FUNC_(ROUND_BEGIN, "к");
			FILL_LIST_OF_FUNC_(ROUND_END,   "лети");
			FILL_LIST_OF_FUNC_(CURLY_BEGIN, "взлёт");
			FILL_LIST_OF_FUNC_(CURLY_END,   "приземление");
			FILL_LIST_OF_FUNC_(COMMENT,     "от_винта");
			FILL_LIST_OF_FUNC_(EQUALLY,     "работает?");
			FILL_LIST_OF_FUNC_(LESS,        "крен");
			FILL_LIST_OF_FUNC_(NO_MORE,     "элерон");
			FILL_LIST_OF_FUNC_(NO_LESS,     "хвост");
			FILL_LIST_OF_FUNC_(NOT_EQUALLY, "керосин");
			FILL_LIST_OF_FUNC_(AND,         "горизонт");
			FILL_LIST_OF_FUNC_(OR,          "вариометр");
			FILL_LIST_OF_FUNC_(NOT,         "крайний");

			default:    //Встретилось значение не существующего значения enum
			{
				value_of_enum += 1;
				continue;
			}
		}

		value_of_enum              += 1;
		list_of_func -> free_index += 1;

		if (list_of_func -> free_index >= list_of_func -> size_of_list)
		{
			status = realloc_list_of_func (list_of_func);
			if (status) {return status;}
		}
	}

	return NOT_ERROR;
}

static front_end_error_t realloc_list_of_func (list_of_func_t* list_of_func)
{
	assert (list_of_func);

	list_of_func -> size_of_list *= 2;

	list_of_func -> array_of_func = (name_base_func_t*) realloc (list_of_func -> array_of_func, sizeof (name_base_func_t) * list_of_func -> size_of_list);

	if (list_of_func -> array_of_func == NULL)
	{
		printf ("\nError in %s:%d\nNot memory for realloc of list_of_func\n\n", __FILE__, __LINE__);
		return NOT_MEMORY_FOR_REALLOC_LIST_OF_FUNC;
	}

	return NOT_ERROR;
}

front_end_error_t dump_list_of_func (list_of_func_t* list_of_func)
{
	assert (list_of_func);

	printf ("\n\n------------------------------------------------------------------\n\n");
	printf ("list_of_func:\n\nsize_of_list == %ld\nfree_index   == %ld\n\n", list_of_func -> size_of_list, list_of_func -> free_index);

	printf ("index    code_func    name_func\n");

	for (size_t index = 0; index < list_of_func -> free_index; index++)
	{
		if ((list_of_func -> array_of_func)[index].code_func != OPERATOR)
			printf ("%5ld    %9d    %s\n", index, (list_of_func -> array_of_func)[index].code_func, 
		                                          (list_of_func -> array_of_func)[index].name_func);
		else
			printf ("%5ld    %9d    '\\n'\n", index, (list_of_func -> array_of_func)[index].code_func);
	}

	printf ("\n\n------------------------------------------------------------------\n\n");

	return NOT_ERROR;
}

long find_word_in_list_of_func (list_of_func_t* list_of_func, char* find_word)
{
	assert (list_of_func);
	assert (find_word);

	for (size_t index = 0; index < list_of_func -> free_index; index++)
	{
		if (strcmp ((list_of_func -> array_of_func)[index].name_func, find_word) == 0)
		{
			return (long) (list_of_func -> array_of_func)[index].code_func;
		}
	}

	return WORD_IS_NOT_FUNC;
}
