#ifndef CONST_IN_FRONT_END_H
#define CONST_IN_FRONT_END_H

#include "const_language.h"

//--------------------------------------------------------------------------------------
/*recursive_descent*/

enum status_of_position_t
{
	SIMPLY_POSITION = 0,
	IN_WHILE        = 1,
	IN_FUNC         = 2
};

enum print_error_in_get_operation_t
{
	NOT_PRINT_ERROR = 0,
	PRINT_ERROR     = 1
};

enum check_declaration_t
{
	NOT_CHECK_DECLARATION = 0,
	CHECK_DECLARATION     = 1
};

struct syntactic_parameters_t
{
	array_of_tokens_t*           tokens;
	name_t*                      array_names;
	list_of_local_name_tables_t* list_of_local_name_tables;
	size_t                       index_token;
	long                         scope;
	status_of_position_t         position;
};


#endif