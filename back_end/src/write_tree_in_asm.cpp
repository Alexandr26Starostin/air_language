#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "const_language.h"
#include "const_in_back_end.h"
#include "operations_with_files.h"
#include "tree.h"
//#include "list_of_func.h"
#include "name_table.h"
#include "local_name_table.h"
#include "write_tree_in_asm.h"

#define WRITE_NODE_(sign, value_type, name_print_func)  if (value_type == sign) {return name_print_func (node, tree_in_asm);}  

#define WRITE_ARITHMETIC_OPERATION_(code_of_operation, str_for_asm)        	\
	if (node -> value.value_keyword == code_of_operation)                  	\
	{                     													\
		fprintf (tree_in_asm -> file_for_asm, str_for_asm);					\
		break;																\
	}

#define WRITE_COMPARISON_OPERATION_(type_of_comparison,  str_with_comparison, str_with_jmp)           	\
	if (node -> value.value_keyword == type_of_comparison)           							 		\
	{																									\
		fprintf (tree_in_asm -> file_for_asm, str_with_comparison);										\
		print_tabulation_in_file (tree_in_asm);															\
		fprintf (tree_in_asm -> file_for_asm, str_with_jmp);											\
																										\
		return NOT_ERROR;																				\
	}

static language_error_t write_node_in_asm                    (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_constant_node                  (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_identifier_node                (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_keyword_node                   (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_function_definition_node       (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_parameters_node                (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_call_node                      (node_t* node, tree_in_asm_t* tree_in_asm);

static language_error_t write_parameters_for_call            (node_t* node, tree_in_asm_t* tree_in_asm);

static language_error_t write_if_in_file                     (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_while_in_file                  (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_assign_in_file                 (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_var_declaration_node           (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_operator_in_file               (node_t* node, tree_in_asm_t* tree_in_asm);   
static language_error_t write_base_func_with_one_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_base_func_with_two_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_printf_in_file                 (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_scanf_in_file                  (node_t* node, tree_in_asm_t* tree_in_asm);  
static language_error_t write_comparison_in_file             (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_abort_in_file                  (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_return_in_asm                  (node_t* node, tree_in_asm_t* tree_in_asm);

static language_error_t print_tabulation_in_file             (tree_in_asm_t* tree_in_asm);
//------------------------------------------------------------------------------------------------------------------------------------

language_error_t write_tree_in_asm (int argc, char** argv, node_t* root_node, name_table_t* name_table, 
                                    list_of_local_name_tables_t* list_of_local_name_tables, char* str_with_table)
{
	assert (argv);
	assert (root_node);
	assert (name_table);
	assert (list_of_local_name_tables);
	assert (str_with_table);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = find_program_file (argc, argv, FIND_FILE_FOR_ASM);
	if (file_for_asm == NULL) {return NOT_FIND_FILE_FOR_ASM;}

	tree_in_asm_t tree_in_asm = {};

	tree_in_asm.file_for_asm              = file_for_asm;
	tree_in_asm.list_of_local_name_tables = list_of_local_name_tables;
	tree_in_asm.name_table                = name_table;
	tree_in_asm.position_in_assign        = NOT_IN_ASSIGN;
	tree_in_asm.index_of_if               = 0;
	tree_in_asm.index_of_while            = 0;
	tree_in_asm.index_of_operator         = 0;
	tree_in_asm.str_with_table            = str_with_table;
	tree_in_asm.depth_of_tabulation       = 0;
	tree_in_asm.scope                     = GLOBAL_SCOPE;
	tree_in_asm.status_of_func            = MAIN_FUNC;

	fprintf (tree_in_asm.file_for_asm, "push %ld    #начальная позиция стекового фрейма\npop BX\n\n", name_table -> free_index);

	status = write_node_in_asm (root_node, &tree_in_asm);
	if (status) {return status;}

	fprintf (tree_in_asm.file_for_asm, "\n\nhlt");

	fclose (file_for_asm);

	return NOT_ERROR;
}

static language_error_t write_node_in_asm (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);
	assert (node);

	WRITE_NODE_(node -> type, CONSTANT,            write_constant_node);
	WRITE_NODE_(node -> type, IDENTIFIER,          write_identifier_node);
	WRITE_NODE_(node -> type, KEYWORD,             write_keyword_node);
	WRITE_NODE_(node -> type, VAR_DECLARATION,     write_var_declaration_node);
	WRITE_NODE_(node -> type, FUNCTION_DEFINITION, write_function_definition_node);
	WRITE_NODE_(node -> type, CALL,                write_call_node);

	return NOT_ERROR;
}

static language_error_t write_constant_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);
	assert (node);

	print_tabulation_in_file (tree_in_asm);

	fprintf (tree_in_asm -> file_for_asm, "push %lg    #кладёт в стек число = %lg\n", node -> value.value_constant, node -> value.value_constant);

	return NOT_ERROR;
}

static language_error_t write_identifier_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);

	size_t index_id_in_name_table = node -> value.value_identifier.index_id_in_name_table;

	if (node -> value.value_identifier.type == NAME_FUNC)     //func (global)
	{
		print_symbols_from_str_in_file (tree_in_asm -> file_for_asm, 
		                                tree_in_asm -> str_with_table + (tree_in_asm -> name_table -> array_names)[index_id_in_name_table].index_to_name_in_str,
																		(tree_in_asm -> name_table -> array_names)[index_id_in_name_table].len_name);

		fprintf (tree_in_asm -> file_for_asm, ":    #создаёт метку с именем функции\n\n");

		return NOT_ERROR;
	}

	print_tabulation_in_file (tree_in_asm);

	//node -> value.value_identifier.type == NAME_VAR

	long index_id_in_scope = find_id_in_scope (tree_in_asm -> list_of_local_name_tables, tree_in_asm -> scope, index_id_in_name_table);

	if (index_id_in_scope == GLOBAL_SCOPE)     //global var
	{
		if (tree_in_asm -> position_in_assign == IN_ASSIGN)
		{
			fprintf (tree_in_asm -> file_for_asm, "pop [%ld]    #присвоил значение глобальной переменной\n\n", index_id_in_name_table);
		}

		else
		{
			fprintf (tree_in_asm -> file_for_asm, "push [%ld]    #вернул значение глобальной переменной\n\n", index_id_in_name_table);
		}

		return NOT_ERROR;
	}

	//index_id_in_scope != GLOBAL_SCOPE    //local var
	//BX - регистр для сохранения позиции начала локальной области видимости 

	if (tree_in_asm -> position_in_assign == IN_ASSIGN)
	{
		fprintf (tree_in_asm -> file_for_asm, "pop [BX+%ld]    #присвоил значение локальной переменной\n\n", index_id_in_scope);
	}

	else
	{
		fprintf (tree_in_asm -> file_for_asm, "push [BX+%ld]    #вернул значение локальной переменной\n\n", index_id_in_scope);
	}

	return NOT_ERROR;
}

static language_error_t print_tabulation_in_file (tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);

	for (size_t index = 0; index < tree_in_asm -> depth_of_tabulation; index++)
	{
		fprintf (tree_in_asm -> file_for_asm, "\t");
	}

	return NOT_ERROR;
}

static language_error_t write_keyword_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	print_tabulation_in_file (tree_in_asm);

	WRITE_NODE_(node -> value.value_keyword, IF,       write_if_in_file);
	WRITE_NODE_(node -> value.value_keyword, WHILE,    write_while_in_file);
	WRITE_NODE_(node -> value.value_keyword, ASSIGN,   write_assign_in_file);
	WRITE_NODE_(node -> value.value_keyword, OPERATOR, write_operator_in_file);

	WRITE_NODE_(node -> value.value_keyword, ADD,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, SUB,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, MUL,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, DIV,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, DEG,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, LOG,      write_base_func_with_two_arg_in_file);

	WRITE_NODE_(node -> value.value_keyword, SIN,      write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, COS,      write_base_func_with_one_arg_in_file);
	//WRITE_NODE_(node -> value.value_keyword, FLOOR,     write_base_func_with_one_arg_in_file);
	//WRITE_NODE_(node -> value.value_keyword, DIFF,      write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, SQRT,     write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, SH,       write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, CH,       write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, LN,       write_base_func_with_one_arg_in_file);

	WRITE_NODE_(node -> value.value_keyword, PRINTF,   write_printf_in_file);
	WRITE_NODE_(node -> value.value_keyword, SCANF,    write_scanf_in_file);

	WRITE_NODE_(node -> value.value_keyword, EQUALLY,     write_comparison_in_file);
	WRITE_NODE_(node -> value.value_keyword, NOT_EQUALLY, write_comparison_in_file);
	WRITE_NODE_(node -> value.value_keyword, MORE,        write_comparison_in_file);
	WRITE_NODE_(node -> value.value_keyword, LESS,        write_comparison_in_file);
	WRITE_NODE_(node -> value.value_keyword, NO_MORE,     write_comparison_in_file);
	WRITE_NODE_(node -> value.value_keyword, NO_LESS,     write_comparison_in_file);

	WRITE_NODE_(node -> value.value_keyword, ABORT,      write_abort_in_file);  
	WRITE_NODE_(node -> value.value_keyword, RETURN,     write_return_in_asm);


	return NOT_ERROR;
}

static language_error_t write_if_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало if\n\n");

	tree_in_asm -> depth_of_tabulation += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#условие if\n\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	size_t old_index_of_if = tree_in_asm -> index_of_if;

	tree_in_asm -> index_of_if += 1;

	fprintf (tree_in_asm -> file_for_asm, "skip_if_%ld:    #если условие не верно, то перепрыгни через тело if\n\n", old_index_of_if);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#тело if\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "skip_if_%ld:    #метка для пропуска if\n\n", old_index_of_if);

	tree_in_asm -> depth_of_tabulation -= 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец if\n\n");

	return NOT_ERROR;
}

static language_error_t write_while_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало while\n\n");

	tree_in_asm -> depth_of_tabulation += 1;
	size_t old_index_of_while = tree_in_asm -> index_of_while;

	tree_in_asm -> index_of_while += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "repeat_while_%ld:    #метка для повторного запуска while\n\n", old_index_of_while);


	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#условие while\n\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	fprintf (tree_in_asm -> file_for_asm, "skip_while_%ld:    #если условие не верно, то перепрыгни через тело while\n\n", old_index_of_while);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#тело while\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "jmp repeat_while_%ld:    #метка для повторного запуска while\n\n", old_index_of_while);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "skip_while_%ld:    #метка для пропуска while\n\n", old_index_of_while);

	tree_in_asm -> depth_of_tabulation -= 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец while\n\n");

	return NOT_ERROR;
}

static language_error_t write_assign_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало assign <variable> = <expression>\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#начало expression\n\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец expression\n\n");

	tree_in_asm -> position_in_assign = IN_ASSIGN;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#начало присваивания переменной\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец присваивания переменной\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец assign\n\n");

	tree_in_asm -> position_in_assign = NOT_IN_ASSIGN;

	return NOT_ERROR;
}

static language_error_t write_var_declaration_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	print_tabulation_in_file (tree_in_asm);

	fprintf (tree_in_asm -> file_for_asm, "#начало var_declaration  double <variable>...\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец var_declaration\n\n");

	return NOT_ERROR;
}

static language_error_t write_operator_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#описание %ld оператора\n\n", tree_in_asm -> index_of_operator);
	tree_in_asm -> index_of_operator   += 1;
	tree_in_asm -> depth_of_tabulation += 1;

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	tree_in_asm -> depth_of_tabulation -= 1;

	if (node -> right)
	{
		status = write_node_in_asm (node -> right, tree_in_asm);
		if (status) {return status;}
	}

	return NOT_ERROR;
}

static language_error_t write_base_func_with_two_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало выполнения арифметической операции с двумя аргументами\n\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);

	for (;;)
	{
		WRITE_ARITHMETIC_OPERATION_(ADD, "add    # exp + exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(SUB, "sub    # exp - exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(MUL, "mul    # exp * exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(DIV, "div    # exp / exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(DEG, "deg    # exp ^ exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(LOG, "log    # log exp exp\n\n");
	}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец выполнения арифметической операции с двумя аргументами\n\n");

	return NOT_ERROR;
}

static language_error_t write_base_func_with_one_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало выполнения арифметической операции с один аргументом\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);

	for (;;)
	{
		WRITE_ARITHMETIC_OPERATION_(SIN,   "sin    # sin exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(COS,   "cos    # cos exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(SQRT,  "sqrt    # sqrt exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(SH,    "sh    # sh exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(CH,    "ch    # ch exp\n\n");
		WRITE_ARITHMETIC_OPERATION_(LN,    "ln    # ln exp\n\n");
		//WRITE_ARITHMETIC_OPERATION_(FLOOR, "floor    # floor exp\n\n");
		//WRITE_ARITHMETIC_OPERATION_(DIFF,  "diff    # diff exp\n\n");
	}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец выполнения арифметической операции с один аргументом\n\n");

	return NOT_ERROR;
}

static language_error_t write_printf_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало печати числа\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "out    #печатает число из стека\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец печати числа\n\n");

	return NOT_ERROR;
}

static language_error_t write_scanf_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	fprintf (tree_in_asm -> file_for_asm, "#начало сканирования числа\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "in    #считывает число и кладёт его в стек\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец сканирования числа\n\n");

	return NOT_ERROR;
}

static language_error_t write_comparison_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало печати сравнения\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#левая часть сравнения\n\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#правая часть сравнения\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#операция сравнения\n\n");

	print_tabulation_in_file (tree_in_asm);

	WRITE_COMPARISON_OPERATION_(EQUALLY,     "# <> == <>\n", "jne ");
	WRITE_COMPARISON_OPERATION_(NOT_EQUALLY, "# <> != <>\n", "je ");
	WRITE_COMPARISON_OPERATION_(NO_MORE,     "# <> <= <>\n", "jb ");
	WRITE_COMPARISON_OPERATION_(LESS,        "# <> <  <>\n", "jbe ");
	WRITE_COMPARISON_OPERATION_(NO_LESS,     "# <> >= <>\n", "ja ");
	WRITE_COMPARISON_OPERATION_(MORE,        "# <> >  <>\n", "jae ");

	return NOT_ERROR;
}

static language_error_t write_abort_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);	

	fprintf (tree_in_asm -> file_for_asm, "hlt #остановка процессора\n\n");

	return NOT_ERROR;
}

static language_error_t write_function_definition_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	size_t index_id_in_name_table = node -> value.value_identifier.index_id_in_name_table;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#начало определения функции\n\n");

	print_tabulation_in_file (tree_in_asm);

	print_symbols_from_str_in_file (tree_in_asm -> file_for_asm, 
		                            tree_in_asm -> str_with_table + (tree_in_asm -> name_table -> array_names)[index_id_in_name_table].index_to_name_in_str,
																	(tree_in_asm -> name_table -> array_names)[index_id_in_name_table].len_name);

	fprintf (tree_in_asm -> file_for_asm, ":    #имя функции\n\n");

	tree_in_asm -> depth_of_tabulation += 1;

	long old_scope       = tree_in_asm -> scope;
	tree_in_asm -> scope = index_id_in_name_table;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#параметры функции\n\n");

	node = node -> right;

	status = write_parameters_node (node -> left, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#тело функции\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец функции\n\n");

	tree_in_asm -> depth_of_tabulation -= 1;

	tree_in_asm -> scope = old_scope;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "hlt    #останавливает процессор\n\n");

	if (tree_in_asm -> status_of_func == MAIN_FUNC)
	{
		tree_in_asm -> status_of_func = CALL_FUNC;
	}

	return NOT_ERROR;
}

static language_error_t write_parameters_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);

	if (node == NULL) {return NOT_ERROR;}

	language_error_t status = NOT_ERROR;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#присваивание значений локальных параметрам функции из стека в оперативную память\n\n");

	size_t index_of_parameter = 1;

	tree_in_asm -> position_in_assign = IN_ASSIGN;

	while (node != NULL)
	{
		print_tabulation_in_file (tree_in_asm);
		fprintf (tree_in_asm -> file_for_asm, "# %ld параметр\n\n", index_of_parameter);

		index_of_parameter += 1; 

		status = write_identifier_node (node -> right, tree_in_asm);
		if (status) {return status;}

		node = node -> left;
	}

	tree_in_asm -> position_in_assign = NOT_IN_ASSIGN;

	return NOT_ERROR;
}

static language_error_t write_return_in_asm (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#завершение функции\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#вычисляет значение, возвращаемое функцией\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "pop AX    #записывает возвращаемое функцией значение\n\n");

	if (tree_in_asm -> status_of_func != MAIN_FUNC)
	{
		print_tabulation_in_file (tree_in_asm);
		fprintf (tree_in_asm -> file_for_asm, "ret    #переход по метке к месту вызова функции\n\n");
	}

	return NOT_ERROR;
}

static language_error_t write_call_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	size_t index_id_in_name_table = node -> right -> value.value_identifier.index_id_in_name_table;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#начинает вызов функции\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "push BX    #сохраняет значение старого стекового фрейма\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#записывает аргументы для функции\n\n");

	status = write_parameters_for_call (node -> left, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#вызывает функцию\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "call ");

	// status = write_identifier_node (node -> right, tree_in_asm);
	// if (status) return status;

	print_symbols_from_str_in_file (tree_in_asm -> file_for_asm, 
		                            tree_in_asm -> str_with_table + (tree_in_asm -> name_table -> array_names)[index_id_in_name_table].index_to_name_in_str,
																	(tree_in_asm -> name_table -> array_names)[index_id_in_name_table].len_name);

	fprintf (tree_in_asm -> file_for_asm, ":    #имя функции\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "pop BX    #записывает предыдущее значение начала стекового фрейма\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "push AX    #получает значение, вернувшееся от функции\n\n");	

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец вызова функции\n\n");

	return NOT_ERROR;
}

static language_error_t write_parameters_for_call (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "# начал записи значений аргументов, передаваемых функции\n\n");

	size_t index_of_parameter = 0;

	node_t* save_node = node;

	while (node != NULL)
	{
		index_of_parameter += 1;

		save_node = node;
		node      = node -> left;
	}

	node = save_node;

	for (size_t index = index_of_parameter; index > 0; index--)
	{
		print_tabulation_in_file (tree_in_asm);
		fprintf (tree_in_asm -> file_for_asm, "# %ld аргумент\n\n", index);

		status = write_node_in_asm (node -> right, tree_in_asm);
		if (status) {return status;}

		node = node -> parent;
	}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец записи значений аргументов, передаваемых функции\n\n");

	return NOT_ERROR;
}

