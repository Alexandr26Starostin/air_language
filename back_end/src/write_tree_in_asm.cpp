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

static language_error_t write_node_in_asm                    (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t print_tabulation_in_file             (tree_in_asm_t* tree_in_asm);
static language_error_t write_constant_node                  (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_identifier_node                (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_keyword_node                   (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_if_in_file                     (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_while_in_file                  (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_assign_in_file                 (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_var_declaration_node           (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_operator_in_file               (node_t* node, tree_in_asm_t* tree_in_asm);   
static language_error_t write_base_func_with_one_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_base_func_with_two_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm); 
static language_error_t write_printf_in_file                 (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_scanf_in_file                  (node_t* node, tree_in_asm_t* tree_in_asm);

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

	status = write_node_in_asm (root_node, &tree_in_asm);
	if (status) {return status;}

	fclose (file_for_asm);

	return NOT_ERROR;
}

static language_error_t write_node_in_asm (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);
	assert (node);

	//if (node == NULL) {return NOT_ERROR;}

	WRITE_NODE_(node -> type, CONSTANT,        write_constant_node);
	WRITE_NODE_(node -> type, IDENTIFIER,      write_identifier_node);
	WRITE_NODE_(node -> type, KEYWORD,         write_keyword_node);
	WRITE_NODE_(node -> type, VAR_DECLARATION, write_var_declaration_node);

	// status = write_node_in_asm (node -> left, tree_in_asm);
	// if (status) {return status;}

	// status = write_node_in_asm (node -> right, tree_in_asm);
	// if (status) {return status;}

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
	assert (node);
	assert (tree_in_asm);

	print_tabulation_in_file (tree_in_asm);

	size_t index_id_in_name_table = node -> value.value_identifier.index_id_in_name_table;

	if (node -> value.value_identifier.type == NAME_FUNC)     //func (global)
	{
		print_tabulation_in_file (tree_in_asm);

		print_symbols_from_str_in_file (tree_in_asm -> file_for_asm, 
		                                tree_in_asm -> str_with_table + (tree_in_asm -> name_table -> array_names)[index_id_in_name_table].index_to_name_in_str,
																		(tree_in_asm -> name_table -> array_names)[index_id_in_name_table].len_name);

		fprintf (tree_in_asm -> file_for_asm, ":    #создаёт метку с именем функции\n");

		return NOT_ERROR;
	}

	//node -> value.value_identifier.type == NAME_VAR

	if (tree_in_asm -> scope == GLOBAL_SCOPE)     //global var
	{
		if (tree_in_asm -> position_in_assign == IN_ASSIGN)
		{
			fprintf (tree_in_asm -> file_for_asm, "pop [%ld]    #присвоил значение глобальной переменной\n", index_id_in_name_table);
		}

		else
		{
			fprintf (tree_in_asm -> file_for_asm, "push [%ld]    #вернул значение глобальной переменной\n", index_id_in_name_table);
		}

		return NOT_ERROR;
	}

	//tree_in_asm -> scope != GLOBAL_SCOPE    //local var
	//BX - регистр для сохранения позиции начала локальной области видимости 

	if (tree_in_asm -> position_in_assign == IN_ASSIGN)
	{
		fprintf (tree_in_asm -> file_for_asm, "pop [BX + %ld]    #присвоил значение локальной переменной\n", index_id_in_name_table);
	}

	else
	{
		fprintf (tree_in_asm -> file_for_asm, "push [BX + %ld]    #вернул значение локальной переменной\n", index_id_in_name_table);
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
	//WRITE_NODE_(node -> value.value_keyword, FLOOR,      write_base_func_with_one_arg_in_file);
	//WRITE_NODE_(node -> value.value_keyword, DIFF,      write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, SQRT,     write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, SH,       write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, CH,       write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, LN,       write_base_func_with_one_arg_in_file);

	WRITE_NODE_(node -> value.value_keyword, PRINTF,   write_printf_in_file);
	WRITE_NODE_(node -> value.value_keyword, SCANF,    write_scanf_in_file);

	return NOT_ERROR;
}

static language_error_t write_if_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало if\n");

	tree_in_asm -> depth_of_tabulation += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#условие if\n\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	fprintf (tree_in_asm -> file_for_asm, "skip_if_%ld:    #если условие не верно, то перепрыгни через тело if\n\n", tree_in_asm -> index_of_if);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#тело if\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "skip_if_%ld:    #метка для пропуска if\n", tree_in_asm -> index_of_if);

	tree_in_asm -> depth_of_tabulation -= 1;
	tree_in_asm -> index_of_if         += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец if\n\n");

	return NOT_ERROR;
}

static language_error_t write_while_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало while\n");

	tree_in_asm -> depth_of_tabulation += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "repeat_while_%ld:    #метка для повторного запуска while\n\n", tree_in_asm -> index_of_while);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#условие while\n\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	fprintf (tree_in_asm -> file_for_asm, "skip_while_%ld:    #если условие не верно, то перепрыгни через тело while\n\n", tree_in_asm -> index_of_while);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#тело while\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "jmp repeat_while_%ld:    #метка для повторного запуска while\n\n", tree_in_asm -> index_of_while);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "skip_while_%ld:    #метка для пропуска while\n", tree_in_asm -> index_of_while);

	tree_in_asm -> depth_of_tabulation -= 1;
	tree_in_asm -> index_of_while      += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец while\n\n");

	return NOT_ERROR;
}

static language_error_t write_assign_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, "#начало assign <variable> = <expression>\n");

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

	fprintf (tree_in_asm -> file_for_asm, "#начало var_declaration  double <variable>...\n");

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
	tree_in_asm -> index_of_operator += 1;

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

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

	fprintf (tree_in_asm -> file_for_asm, "#начало выполнения арифметической операции с двумя аргументами\n");

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);

	for (;;)
	{
		WRITE_ARITHMETIC_OPERATION_(ADD, "add    # exp + exp");
		WRITE_ARITHMETIC_OPERATION_(SUB, "sub    # exp - exp");
		WRITE_ARITHMETIC_OPERATION_(MUL, "mul    # exp * exp");
		WRITE_ARITHMETIC_OPERATION_(DIV, "div    # exp / exp");
		WRITE_ARITHMETIC_OPERATION_(DEG, "deg    # exp ^ exp");
		WRITE_ARITHMETIC_OPERATION_(LOG, "log    # log exp exp");
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

	fprintf (tree_in_asm -> file_for_asm, "#начало выполнения арифметической операции с один аргументом\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);

	for (;;)
	{
		WRITE_ARITHMETIC_OPERATION_(SIN,   "sin    # sin exp");
		WRITE_ARITHMETIC_OPERATION_(COS,   "cos    # cos exp");
		WRITE_ARITHMETIC_OPERATION_(SQRT,  "sqrt    # sqrt exp");
		WRITE_ARITHMETIC_OPERATION_(SH,    "sh    # sh exp");
		WRITE_ARITHMETIC_OPERATION_(CH,    "ch    # ch exp");
		WRITE_ARITHMETIC_OPERATION_(LN,    "ln    # ln exp");
		//WRITE_ARITHMETIC_OPERATION_(FLOOR, "floor    # floor exp");
		//WRITE_ARITHMETIC_OPERATION_(DIFF,  "diff    # diff exp");
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

	fprintf (tree_in_asm -> file_for_asm, "#начало печати числа\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "out    #печатает число из стека\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец печати числа\n\n");

	return NOT_ERROR;
}

static language_error_t write_scanf_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	fprintf (tree_in_asm -> file_for_asm, "#начало сканирования числа\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "in    #считывает число и кладёт его в стек\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, "#конец сканирования числа\n\n");

	return NOT_ERROR;
}