#ifndef CONST_LANGUAGE_H
#define CONST_LANGUAGE_H

const size_t MAX_LEN_WORD_FROM_STR = 50;

//------------------------------------------------------------------------------
/*errors*/

enum language_error_t
{
	NOT_ERROR 				                 		 = 0,
	NOT_MEMORY_FOR_LIST_OF_FUNC              		 = 1,
	NOT_MEMORY_FOR_REALLOC_LIST_OF_FUNC      		 = 2,
	NOT_MEMORY_FOR_STR                      		 = 4,
	NOT_MEMORY_FOR_NAME_TABLE                		 = 5,
	NOT_MEMORY_FOR_REALLOC_NAME_TABLE        		 = 6,
	NOT_MEMORY_FOR_ARRAY_OF_TOKENS           		 = 7,
	NOT_MEMORY_FOR_REALLOC_ARRAY_OF_TOKENS   		 = 8,
	NOT_MEMORY_FOR_NEW_NODE                  		 = 9,
	NOT_FIND_TREE_DOT                                = 10,
	NOT_FIND_TREE_HTML                               = 11,
	NOT_MEMORY_FOR_ARRAY_OF_LOCAL_NAME_TABLE         = 12,
	NOT_MEMORY_FOR_ARRAY_OF_LOCAL_NAMES              = 13,
	NOT_MEMORY_FOR_REALLOC_ARRAY_OF_LOCAL_NAME_TABLE = 14,
	NOT_MEMORY_FOR_REALLOC_ARRAY_OF_LOCAL_NAMES      = 15,
	NOT_FIND_LOCAL_NAME_TABLE_WITH_SCOPE             = 16,
	NOT_FIND_FILE_WITH_PROGRAM               		 = 3,
	NOT_FIND_FILE_WITH_TREE    					     = 17,   
	NOT_FIND_FILE_WITH_TABLE                         = 18,   
	NOT_FIND_FILE_FOR_TREE                           = 19,   
	NOT_FIND_FILE_FOR_TABLE                          = 20,
	NOTFIND_FILE_FOR_ASM                             = 21, 
	ERROR_IN_CREATE_TREE_FROM_STR                    = 22, 
	ERROR_IN_CREATE_TABLE_FROM_STR                   = 23, 

	ERROR_IN_GET_GRAMMAR                   = 30,
	ERROR_IN_GET_OPERATION                 = 31,
	ERROR_IN_GET_ASSIGN                    = 32,
	ERROR_IN_GET_IF                        = 33,
	ERROR_IN_GET_WHILE                     = 34,
	ERROR_IN_GET_PRINTF                    = 35,
	ERROR_IN_GET_SCANF                     = 36,
	ERROR_IN_GET_INTERRUPTION              = 37,
	ERROR_IN_GET_EMPTY_OPERATION           = 38,
	ERROR_IN_GET_DEFINITION_FUNC           = 39,
	ERROR_IN_GET_RETURN                    = 40,
	ERROR_IN_GET_GLOBAL_VARIABLE           = 41,
	ERROR_IN_GET_LOGICAL                   = 42,
	ERROR_IN_GET_PARAMETERS_FOR_DEFINITION = 43,
	ERROR_IN_GET_PARAMETERS_FOR_CALL       = 44,
	ERROR_IN_GET_EXPRESSION                = 45,
	ERROR_IN_GET_TERM                      = 46,
	ERROR_IN_GET_POWER                     = 47,
	ERROR_IN_GET_BASE_FUNC                 = 48,
	ERROR_IN_GET_ROUND                     = 49,
	ERROR_IN_GET_ELEMENT                   = 50,
	ERROR_IN_GET_CONSTANT                  = 51,
	ERROR_IN_GET_VARIABLE                  = 52,
	ERROR_IN_GET_CALL_FUNC                 = 53,
	ERROR_IN_GET_VAR_DECLARATION           = 54,

	SKIP_GET_ASSIGN          = 70,
	SKIP_GET_IF              = 71,
	SKIP_GET_WHILE           = 72,
	SKIP_GET_PRINTF          = 73,
	SKIP_GET_SCANF           = 74,
	SKIP_GET_INTERRUPTION    = 75,
	SKIP_GET_EMPTY_OPERATION = 76,
	SKIP_GET_DEFINITION_FUNC = 77,
	SKIP_GET_RETURN          = 78,
	SKIP_GET_GLOBAL_VARIABLE = 79,
	SKIP_GET_VAR_DECLARATION = 80

};

//--------------------------------------------------------------------------------
/*list_of_func*/

const size_t SIZE_LIST_OF_FUNC = 5;
const size_t MAX_VALUE_OF_ENUM = 120;
const long   WORD_IS_NOT_FUNC  = -1;

enum base_func_t
{
	//------------------------------------------------------------------------------------------
	/*standard*/

	IF       = 11,      // if 
	WHILE    = 12,      // while
	ASSIGN   = 13,      // =

	SIN = 21,   // sin
	COS = 22,   // cos

	//FLOOR = 23,

	ADD = 24,   // +
	SUB = 25,   // -
	MUL = 26,   // *
	DIV = 27,   // /

	//DIFF = 28,

	SQRT = 29,    // sqrt

	EQUALLY     = 31,    // ==
	LESS        = 32,    // <
	MORE        = 33,    // >
	NO_MORE     = 34,    // <=
	NO_LESS     = 35,    // >=
	NOT_EQUALLY = 36,    // !=
	AND         = 37,    // &&
	OR          = 38,    // ||
	NOT         = 39,    // !

	OPERATOR = 41,      // ;
	COMMA    = 42,

	INT = 51,      // int/long/doubles

	SCANF  = 61,      // scanf
	PRINTF = 62,      // printf

	RETURN = 71,      // return

	BREAK    = 72,
	CONTINUE = 73,
	ABORT    = 74,

	//-------------------------------------------------------------------------------
	/*other*/
	
	SH = 81,   // sinh
	CH = 82,   // cosh

	LOG = 83,    // log
	LN  = 84,   // ln
	DEG = 85,   // ^

	MAIN     = 86,      // main
	
	ROUND_BEGIN = 91,   // (
	ROUND_END   = 92,   // )
	CURLY_BEGIN = 93,   // {
	CURLY_END   = 94,   // }

	COMMENT = 95    // //

	// -> = 96,
};

struct name_base_func_t
{
	const char* name_func;
	base_func_t code_func;
};

struct list_of_func_t
{
	name_base_func_t* array_of_func;
	size_t            size_of_list;
	size_t            free_index;
};

//---------------------------------------------------------------------------------------
/*operations_with_files*/

enum operation_with_file_t
{
	FIND_FILE_WITH_PROGRAM = 0,   // -read_program
	FIND_FILE_WITH_TREE    = 1,   // -read_tree
	FIND_FILE_WITH_TABLE   = 2,   // -read_table
	FIND_FILE_FOR_TREE     = 3,   // -write_tree
	FIND_FILE_FOR_TABLE    = 4    // -write_table
	//FIND_FILE_FOR_ASM      = 5    // -write_asm
};

// struct tree_from_str_t
// {
// 	char* str_with_tree;
// };

//---------------------------------------------------------------------------------------
/*name_table*/

const size_t SIZE_NAME_TABLE = 5;

enum type_id_t
{
	UNKNOW_TYPE = 0,
	NAME_VAR    = 1,
	NAME_FUNC   = 2,
	NAME_TYPE   = 4
};

enum status_of_declaration_t
{
	NOT_DEFINITE       = 0,
	DEFINITE_IN_GLOBAL = 1,
	DEFINITE_IN_FUNC   = 2
};

struct name_t
{
	size_t                  index_to_name_in_str;
	size_t                  len_name;
	type_id_t               type;
	status_of_declaration_t status;
};

struct name_table_t
{
	name_t* array_names;
	size_t  size_of_name_table;
	size_t  free_index;
};

//--------------------------------------------------------------------------------------
/*tokens*/

const size_t SIZE_ARRAY_OF_TOKENS = 5;

enum token_type_t 
{
	NUM = 1,
	OP  = 2,
	ID  = 3
};


union token_value_t
{
	double      value_num;
	base_func_t value_op;
	size_t      value_id; 
};

struct token_t 
{
	token_type_t        type;
	union token_value_t value;
};

struct array_of_tokens_t
{
	token_t* array_of_tokens;
	size_t   size_of_array;
	size_t   index_free;
};

//---------------------------------------------------------------------------------------
/*tree*/

const size_t INDEX_NUMBER_OF_PICTURE = 36;

enum node_type_t 
{
	CONSTANT            = 1,
	IDENTIFIER          = 2,
	KEYWORD             = 3,
	FUNCTION_DEFINITION = 4,
	PARAMETERS          = 5,
	VAR_DECLARATION     = 6,
	CALL                = 7
};

struct identifier_t 
{
	size_t    index_id_in_name_table;
	type_id_t type;
	long      scope;	
};

union node_value_t
{
	double       value_constant;
	identifier_t value_identifier;
	base_func_t  value_keyword;
	size_t       value_function_definition;
	double       value_parameters;            //not value_parameters
	size_t       value_var_declaration;
	double       value_call;                  //not value_call
};

struct node_t
{
	node_type_t  type;
	node_value_t value;
	node_t*      left;
	node_t*      right;
	node_t*      parent;
};

struct dump_tree_t 
{
	char*              str_for_system; 
	char*              str_with_program; 
	size_t             index_picture;
	FILE*              tree_html; 
	list_of_func_t*    list_of_func; 
	name_table_t*      name_table;
};

//--------------------------------------------------------------------------------------
/*local_name_table*/

const size_t MIN_SIZE_ARRAY_OF_LOCAL_NAME_TABLE = 1;
const size_t MIN_SIZE_ARRAY_OF_LOCAL_NAMES      = 1;
const long   GLOBAL_SCOPE                       = -1;
const long   NOT_DEFINITE_SCOPE                 = -2;

struct local_name_t
{
	size_t    index_id_in_name_table;
	type_id_t type_id;
};

struct local_name_table_t
{
	local_name_t* array_of_local_names;
	size_t        size_local_name_table;
	size_t        free_index_in_local_name_table;
	long          scope_of_local_name_table;
};

struct list_of_local_name_tables_t
{
	local_name_table_t* array_of_local_name_table;
	size_t              size_list;
	size_t              free_index_in_list;
};

//---------------------------------------------------------------------------------------

#endif