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
#include "write_tree_in_nasm.h"

#define WRITE_NODE_(sign, value_type, name_print_func)  if (value_type == sign) {return name_print_func (node, tree_in_asm);}  

// #define WRITE_ARITHMETIC_OPERATION_(code_of_operation, str_for_asm)        	
// 	if (value_in_node == code_of_operation)                  				
// 	{                     												
// 		fprintf (file_for_asm, "", str_for_asm);							
// 		break;																
// 	}

#define WRITE_COMPARISON_OPERATION_(type_of_comparison,  str_with_comparison, str_with_jmp)           	\
	if (node_value  == type_of_comparison)           							 						\
	{																									\
		fprintf (file_for_asm, str_with_comparison);													\
		print_tabulation_in_file (tree_in_asm);															\
		fprintf (file_for_asm, str_with_jmp);															\
																										\
		return NOT_ERROR;																				\
	}

static language_error_t write_node_in_asm                    (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_constant_node                  (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_identifier_node                (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_keyword_node                   (node_t* node, tree_in_asm_t* tree_in_asm);
static language_error_t write_function_definition_node       (node_t* node, tree_in_asm_t* tree_in_asm);
//static language_error_t write_parameters_node                (node_t* node, tree_in_asm_t* tree_in_asm);
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

language_error_t write_tree_in_nasm (int argc, char** argv, node_t* root_node, name_table_t* name_table, 
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

	fprintf (file_for_asm, "global _start     ;начало трансляции в NASM\n"
						   "section .text\n"
						   "_start:\n\n"
						   
						   "\t;--------------------------------------------------------------------------------------------\n"
						   "\t;Выделение места под глобальные переменные  (%s:%d)\n"
						   "\t;--------------------------------------------------------------------------------------------\n"
						   "\tsub rsp, %ld * 8    ;выделение места под глобальные переменные (их количество == %ld)\n"
						   "\tmov rbx, rsp        ;rbx = указатель на глобальные переменные\n" 
						   "\tmov r12, rsp\n"
						   "\t;--------------------------------------------------------------------------------------------\n\n",
						   	__FILE__, __LINE__, name_table -> free_index, name_table -> free_index);
	//fprintf (file_for_asm, "push %ld    #начальная позиция стекового фрейма\npop BX\n\n", name_table -> free_index);

	status = write_node_in_asm (root_node, &tree_in_asm);
	if (status) {return status;}

	
	fprintf (file_for_asm, "\t;--------------------------------------------------------------------------------------------\n"
						   "\t;Завершение работы программы    (%s:%d)\n"
						   "\t;Возвращает значение, которое к этом моменту хранится в rax\n"
						   "\t;--------------------------------------------------------------------------------------------\n"
						   "\tmov rdi, rax     ;exit64 (rdi)\n"
						   "\tmov rax, 0x3C\n"
            			   "\tsyscall\n"
						   ";--------------------------------------------------------------------------------------------\n\n"
						   "%%INCLUDE \"std_lib.s\"\n", 
						   __FILE__, __LINE__);
	// fprintf (tree_in_asm.file_for_asm, "\n\nhlt");


	fclose (file_for_asm);

	return NOT_ERROR;
}

static language_error_t write_node_in_asm (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);
	assert (node);

	node_type_t type = node -> type;

	WRITE_NODE_(type, CONSTANT,            write_constant_node);
	WRITE_NODE_(type, IDENTIFIER,          write_identifier_node);
	WRITE_NODE_(type, KEYWORD,             write_keyword_node);
	WRITE_NODE_(type, VAR_DECLARATION,     write_var_declaration_node);
	WRITE_NODE_(type, FUNCTION_DEFINITION, write_function_definition_node);
	WRITE_NODE_(type, CALL,                write_call_node);

	return NOT_ERROR;
}

static language_error_t write_constant_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);
	assert (node);

	print_tabulation_in_file (tree_in_asm);

	FILE* file_for_asm = tree_in_asm -> file_for_asm;
	assert (file_for_asm);

	long constant = (long) node -> value.value_constant;

	fprintf (file_for_asm, "mov r8, %ld   ;временно сохраняет значение константы = %ld   (%s:%d)\n", 
							constant, constant, __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);

	fprintf (file_for_asm, "push r8   ;кладёт значение константы в стек     (%s:%d)\n\n", 
							__FILE__, __LINE__);
	//fprintf (file_for_asm, "push %lg    #кладёт в стек число = %lg\n", node -> value.value_constant, node -> value.value_constant);

	return NOT_ERROR;
}

static language_error_t write_identifier_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);

	size_t index_id_in_name_table = node -> value.value_identifier.index_id_in_name_table;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;
	assert (file_for_asm);

	name_t name_in_node = (tree_in_asm -> name_table -> array_names)[index_id_in_name_table];

	if (node -> value.value_identifier.type == NAME_FUNC)     //func (global)
	{
		print_symbols_from_str_in_file (file_for_asm, 
		                                tree_in_asm -> str_with_table + name_in_node.index_to_name_in_str,
										name_in_node.len_name);

		fprintf (file_for_asm, ":    ;создаёт метку с именем функции (%s:%d)\n\n", __FILE__, __LINE__);

		return NOT_ERROR;
	}

	print_tabulation_in_file (tree_in_asm);

	//node -> value.value_identifier.type == NAME_VAR

	long index_id_in_scope = find_id_in_scope (tree_in_asm -> list_of_local_name_tables, tree_in_asm -> scope, index_id_in_name_table);

	if (index_id_in_scope == GLOBAL_SCOPE)     //global var
	{
		if (tree_in_asm -> position_in_assign == IN_ASSIGN)
		{
			fprintf (file_for_asm, "pop r8    ;вытащил значение константы   (%s:%d)\n", __FILE__, __LINE__);

			print_tabulation_in_file (tree_in_asm);

			fprintf (file_for_asm, "mov [rbx + %ld * 8], r8    ;присвоил значение глобальной переменной   (%s:%d)\n\n", 
									index_id_in_name_table, __FILE__, __LINE__);
			//fprintf (tree_in_asm -> file_for_asm, "pop [%ld]    #присвоил значение глобальной переменной\n\n", index_id_in_name_table);
		}

		else
		{
			fprintf (file_for_asm, "mov r9, [rbx + %ld * 8]      ;вытащил значение глобальной переменной   (%s:%d)\n", 
									index_id_in_name_table, __FILE__, __LINE__);

			print_tabulation_in_file (tree_in_asm);

			fprintf (file_for_asm, "push r9    ;сохранил значение глобальной переменной в стеке   (%s:%d)\n\n", __FILE__, __LINE__);


			//fprintf (tree_in_asm -> file_for_asm, "push [%ld]    #вернул значение глобальной переменной\n\n", index_id_in_name_table);
		}

		return NOT_ERROR;
	}

	//index_id_in_scope != GLOBAL_SCOPE    //local var
	//BX/r12 - регистр для сохранения позиции начала локальной области видимости 

	if (tree_in_asm -> position_in_assign == IN_ASSIGN)
	{
		fprintf (file_for_asm, "pop r8   ;вытащил константу из стека    (%s:%d)\n", __FILE__, __LINE__);

		print_tabulation_in_file (tree_in_asm);

		fprintf (file_for_asm, "mov [r12 + %ld * 8], r8   ;присвоил значение локальной переменной    (%s:%d)\n\n", 
								index_id_in_scope, __FILE__, __LINE__);
		//fprintf (tree_in_asm -> file_for_asm, "pop [BX+%ld]    #присвоил значение локальной переменной\n\n", index_id_in_scope);
	}

	else
	{
		//fprintf (tree_in_asm -> file_for_asm, "push [BX+%ld]    #вернул значение локальной переменной\n\n", index_id_in_scope);
		fprintf (file_for_asm, "mov r9, [r12 + %ld * 8]      ;вытащил значение локальной переменной   (%s:%d)\n", 
								index_id_in_scope, __FILE__, __LINE__);

		print_tabulation_in_file (tree_in_asm);

		fprintf (file_for_asm, "push r9    ;сохранил значение локальной переменной в стеке   (%s:%d)\n\n", __FILE__, __LINE__);
	}

	return NOT_ERROR;
}

static language_error_t print_tabulation_in_file (tree_in_asm_t* tree_in_asm)
{
	assert (tree_in_asm);

	size_t max_index = tree_in_asm -> depth_of_tabulation;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;
	assert (file_for_asm);

	for (size_t index = 0; index < max_index; index++)
	{
		fprintf (file_for_asm, "\t");
	}

	return NOT_ERROR;
}

static language_error_t write_keyword_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	print_tabulation_in_file (tree_in_asm);

	WRITE_NODE_(node -> value.value_keyword, IF,       write_if_in_file);                   ///////////////////////////
	WRITE_NODE_(node -> value.value_keyword, WHILE,    write_while_in_file);
	WRITE_NODE_(node -> value.value_keyword, ASSIGN,   write_assign_in_file);
	WRITE_NODE_(node -> value.value_keyword, OPERATOR, write_operator_in_file);

	WRITE_NODE_(node -> value.value_keyword, ADD,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, SUB,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, MUL,      write_base_func_with_two_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, DIV,      write_base_func_with_two_arg_in_file);
	// WRITE_NODE_(node -> value.value_keyword, DEG,      write_base_func_with_two_arg_in_file);
	// WRITE_NODE_(node -> value.value_keyword, LOG,      write_base_func_with_two_arg_in_file);

	// WRITE_NODE_(node -> value.value_keyword, SIN,      write_base_func_with_one_arg_in_file);
	// WRITE_NODE_(node -> value.value_keyword, COS,      write_base_func_with_one_arg_in_file);
	// //WRITE_NODE_(node -> value.value_keyword, FLOOR,     write_base_func_with_one_arg_in_file);
	// //WRITE_NODE_(node -> value.value_keyword, DIFF,      write_base_func_with_one_arg_in_file);
	WRITE_NODE_(node -> value.value_keyword, SQRT,     write_base_func_with_one_arg_in_file);
	// WRITE_NODE_(node -> value.value_keyword, SH,       write_base_func_with_one_arg_in_file);
	// WRITE_NODE_(node -> value.value_keyword, CH,       write_base_func_with_one_arg_in_file);
	// WRITE_NODE_(node -> value.value_keyword, LN,       write_base_func_with_one_arg_in_file);

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

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, ";начало if  (%s:%d)\n\n", __FILE__, __LINE__);

	tree_in_asm -> depth_of_tabulation += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";условие if   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	size_t old_index_of_if = tree_in_asm -> index_of_if;

	tree_in_asm -> index_of_if += 1;

	fprintf (file_for_asm, "skip_if_%ld    ;если условие не верно, то перепрыгни через тело if (%s:%d)\n\n", 
							old_index_of_if, __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";тело if  (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "skip_if_%ld:    ;метка для пропуска if    (%s:%d)\n\n", 
							old_index_of_if, __FILE__, __LINE__);

	tree_in_asm -> depth_of_tabulation -= 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец if   (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

static language_error_t write_while_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	language_error_t status = NOT_ERROR;

	fprintf (file_for_asm, ";начало while   (%s:%d)\n\n", __FILE__, __LINE__);

	tree_in_asm -> depth_of_tabulation += 1;
	size_t old_index_of_while = tree_in_asm -> index_of_while;

	tree_in_asm -> index_of_while += 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "repeat_while_%ld:    ;метка для повторного запуска while   (%s:%d)\n\n", 
							old_index_of_while, __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";условие while    (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	fprintf (file_for_asm, "skip_while_%ld   ;если условие не верно, то перепрыгни через тело while   (%s:%d)\n\n", 
							old_index_of_while, __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";тело while    (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "jmp repeat_while_%ld    ;прыжок на повторный запуск while   (%s:%d)\n\n", \
							old_index_of_while, __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "skip_while_%ld:    ;метка для пропуска while   (%s:%d)\n\n", 
							old_index_of_while, __FILE__, __LINE__);

	tree_in_asm -> depth_of_tabulation -= 1;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец while  (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

static language_error_t write_assign_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, ";начало assign <variable> = <expression>   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";начало expression   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец expression  (%s:%d)\n\n", __FILE__, __LINE__);

	tree_in_asm -> position_in_assign = IN_ASSIGN;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";начало присваивания переменной   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец присваивания переменной   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец assign   (%s:%d)\n\n", __FILE__, __LINE__);

	tree_in_asm -> position_in_assign = NOT_IN_ASSIGN;

	return NOT_ERROR;
}

static language_error_t write_var_declaration_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	print_tabulation_in_file (tree_in_asm);

	FILE* file_for_asm = tree_in_asm -> file_for_asm;
	assert (file_for_asm);

	fprintf (file_for_asm, ";начало var_declaration  long <variable>...   (%s:%d)\n\n", __FILE__, __LINE__);

	//fprintf (tree_in_asm -> file_for_asm, "#начало var_declaration  double <variable>...\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);

	fprintf (file_for_asm, ";конец var_declaration  long <variable>...   (%s:%d)\n\n", __FILE__, __LINE__);
	//fprintf (tree_in_asm -> file_for_asm, "#конец var_declaration\n\n");

	return NOT_ERROR;
}

static language_error_t write_operator_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, ";описание %ld оператора  (%s:%d)\n\n", 
							tree_in_asm -> index_of_operator, __FILE__, __LINE__);
	
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

static language_error_t write_base_func_with_two_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm)          /////////////////
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status        = NOT_ERROR;
	FILE*            file_for_asm  = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, ";начало выполнения арифметической операции с двумя аргументами   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";получение правого аргумента   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";получение левого аргумента   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	long  value_in_node = node        -> value.value_keyword;

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "mov r10, rax      ;сохраняет rax   (%s:%d)\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "pop rax   ;вытаскивает первый аргумент (в него же и сохранит результат)   (%s:%d)\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "pop rdx   ;вытаскивает второй аргумент    (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);

	for (;;)
	{
		// WRITE_ARITHMETIC_OPERATION_(ADD, "add    # exp + exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(SUB, "sub    # exp - exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(MUL, "mul    # exp * exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(DIV, "div    # exp / exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(DEG, "deg    # exp ^ exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(LOG, "log    # log exp exp\n\n");

		if (value_in_node == ADD)                  				
		{                     													
			fprintf (file_for_asm, "add rax, rdx     ;rax += rdx   (%s:%d)\n\n", __FILE__, __LINE__);							
			break;																
		}

		if (value_in_node == SUB)                  				
		{                     													
			fprintf (file_for_asm, "sub rax, rdx     ;rax -= rdx     (%s:%d)\n\n", __FILE__, __LINE__);							
			break;																
		}

		if (value_in_node == MUL)                  				
		{                     													
			fprintf (file_for_asm, "imul edx  ;rdx:rax = rax * rdx (берём значения, что rdx:rax == rax)     (%s:%d)\n\n", 
									__FILE__, __LINE__);							
			break;																
		}

		if (value_in_node == DIV)                  				
		{
			fprintf (file_for_asm, "mov r11, rdx   ;сохраняет rdx   (%s:%d)\n", __FILE__, __LINE__);

			print_tabulation_in_file (tree_in_asm);
			fprintf (file_for_asm, "cdq    ;rax -> rdx:rax   (%s:%d)\n", __FILE__, __LINE__);

			print_tabulation_in_file (tree_in_asm);
			fprintf (file_for_asm, "idiv r11d     ;rax = rdx:rax / r11       rdx = rdx:rax %% r11   (%s:%d)\n\n", __FILE__, __LINE__);							

			break;																
		}

		break;
	}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "push rax    ;кладёт в стек результат операции   (%s:%d)\n", __FILE__, __LINE__);

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "mov rax, r10    ;возвращает старое значение rax   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, ";конец выполнения арифметической операции с двумя аргументами  (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

static language_error_t write_base_func_with_one_arg_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	fprintf (tree_in_asm -> file_for_asm, ";начало выполнения арифметической операции с один аргументом\n\n");

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	FILE* file_for_asm  = tree_in_asm -> file_for_asm;
	long  value_in_node = node        -> value.value_keyword;

	print_tabulation_in_file (tree_in_asm);

	for (;;)
	{
		//WRITE_ARITHMETIC_OPERATION_(SIN,   "sin    # sin exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(COS,   "cos    # cos exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(SQRT,  "sqrt    # sqrt exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(SH,    "sh    # sh exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(CH,    "ch    # ch exp\n\n");
		// WRITE_ARITHMETIC_OPERATION_(LN,    "ln    # ln exp\n\n");
		//WRITE_ARITHMETIC_OPERATION_(FLOOR, "floor    # floor exp\n\n");
		//WRITE_ARITHMETIC_OPERATION_(DIFF,  "diff    # diff exp\n\n");

		if (value_in_node == SQRT)
		{
			fprintf (file_for_asm, "pop rdi   ;   (%s:%d)\n", __FILE__, __LINE__);

			print_tabulation_in_file (tree_in_asm);
			fprintf (file_for_asm, "call sqrt_number    ;   (%s:%d)\n", __FILE__, __LINE__);

			print_tabulation_in_file (tree_in_asm);
			fprintf (file_for_asm, "push rax   ;   (%s:%d)\n", __FILE__, __LINE__);		

			break;
		}

		break;
	}

	print_tabulation_in_file (tree_in_asm);
	fprintf (tree_in_asm -> file_for_asm, ";конец выполнения арифметической операции с один аргументом\n\n");

	return NOT_ERROR;
}

static language_error_t write_printf_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, ";начало печати числа   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "mov rdi, str_for_printf    ;   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "pop rsi    ;получает число для печати из стека   (%s:%d)\n\n", __FILE__, __LINE__);  

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "call printf_number    ;печатает число в rdi    (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец печати числа    (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

static language_error_t write_scanf_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, ";начало сканирования числа  (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "call scanf_number    ;считывает число и кладёт его в rax  (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "push rax       ;кладёт считанное число в стек   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец сканирования числа   (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

static language_error_t write_comparison_in_file (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, ";начало печати сравнения   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";правая часть сравнения   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";левая часть сравнения   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> left, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "pop r13   ;r13 = левая часть сравнения   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "pop r14   ;r14 = правая часть сравнения  (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "cmp r13, r14 ;сравнение   (<левая> - <правая>)  (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);

	long node_value = node -> value.value_keyword;

	WRITE_COMPARISON_OPERATION_(EQUALLY,     "; <л> - <п> == 0    (!= то прыжок мимо - jne)\n", "jne ");
	WRITE_COMPARISON_OPERATION_(NOT_EQUALLY, "; <л> - <п> != 0    (== то прыжок мимо - je)\n",  "je ");
	WRITE_COMPARISON_OPERATION_(NO_MORE,     "; <л> - <п> <= 0    (>  то прыжок мимо - jg)\n",  "jg ");
	WRITE_COMPARISON_OPERATION_(LESS,        "; <л> - <п> <  0    (>= то прыжок мимо - jge)\n", "jge ");
	WRITE_COMPARISON_OPERATION_(NO_LESS,     "; <л> - <п> >= 0    (<  то прыжок мимо - jl)\n",  "jl ");
	WRITE_COMPARISON_OPERATION_(MORE,        "; <л> - <п> >  0    (<= то прыжок мимо - jle)\n", "jle ");

	return NOT_ERROR;
}

static language_error_t write_abort_in_file (node_t* node, tree_in_asm_t* tree_in_asm) 
{
	assert (node);
	assert (tree_in_asm);	

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	fprintf (file_for_asm, "call end_program  ;завершает работу программу  (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

static language_error_t write_function_definition_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	size_t index_id_in_name_table = node         -> value.value_identifier.index_id_in_name_table;
	FILE*  file_for_asm           = tree_in_asm  -> file_for_asm;
	name_t name_in_node           = (tree_in_asm -> name_table -> array_names)[index_id_in_name_table];


	tree_in_asm -> depth_of_tabulation = 0;
	//print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";-----------------------------------------------------------------------\n"
						   ";описание функции    (%s:%d)\n"
						   ";-----------------------------------------------------------------------\n\n", __FILE__, __LINE__);
	//fprintf (tree_in_asm -> file_for_asm, "#начало определения функции\n\n");

	//print_tabulation_in_file (tree_in_asm);

	print_symbols_from_str_in_file (file_for_asm, 
		                            tree_in_asm -> str_with_table + name_in_node.index_to_name_in_str,
									name_in_node.len_name);

	fprintf (file_for_asm, ":    ;имя функции   (%s:%d)\n\n", __FILE__, __LINE__);

	tree_in_asm -> depth_of_tabulation += 1;

	long old_scope       = tree_in_asm -> scope;
	tree_in_asm -> scope = index_id_in_name_table;

	size_t index_local_table_in_list = find_position_of_local_table (tree_in_asm -> list_of_local_name_tables, index_id_in_name_table);

	local_name_table_t* array_of_local_name_table = tree_in_asm -> list_of_local_name_tables -> array_of_local_name_table;
	size_t              count_local_arg_in_func   = array_of_local_name_table[index_local_table_in_list].free_index_in_local_name_table;

	if (tree_in_asm -> status_of_func == MAIN_FUNC)
	{
		print_tabulation_in_file (tree_in_asm);
		fprintf (file_for_asm, "sub r12, %ld * 8  ;(%s:%d)\n\n", count_local_arg_in_func, __FILE__, __LINE__);

		print_tabulation_in_file (tree_in_asm);
		fprintf (file_for_asm, "mov rsp, r12  ;(%s:%d)\n\n", __FILE__, __LINE__);
	}

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, ";вытаскивает параметры функции     (%s:%d)\n\n", __FILE__, __LINE__);  

	node = node -> right;

	// status = write_parameters_node (node -> left, tree_in_asm);
	// if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";тело функции   (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец функции   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "add r12, 8*%ld   ;  (%s:%d)\n\n", count_local_arg_in_func, __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "pop r12    ;   (%s:%d)\n\n", __FILE__, __LINE__);

	//tree_in_asm -> depth_of_tabulation -= 1;

	tree_in_asm -> scope = old_scope;

	if (tree_in_asm -> status_of_func == MAIN_FUNC)
	{
		print_tabulation_in_file (tree_in_asm);
		fprintf (file_for_asm, "call end_program  ;завершает работу программу  (%s:%d)\n\n", __FILE__, __LINE__);
		tree_in_asm -> status_of_func = CALL_FUNC;
	}

	return NOT_ERROR;
}

// static language_error_t write_parameters_node (node_t* node, tree_in_asm_t* tree_in_asm)
// {
// 	assert (tree_in_asm);

// 	if (node == NULL) {return NOT_ERROR;}

// 	language_error_t status = NOT_ERROR;

// 	FILE* file_for_asm = tree_in_asm  -> file_for_asm;

// 	//print_tabulation_in_file (tree_in_asm);
// 	//fprintf (tree_in_asm -> file_for_asm, "#присваивание значений локальных параметрам функции из стека в оперативную память\n\n");

// 	size_t index_of_parameter = 1;

// 	tree_in_asm -> position_in_assign = IN_ASSIGN;

// 	while (node != NULL)
// 	{
// 		print_tabulation_in_file (tree_in_asm);
// 		fprintf (file_for_asm, "; %ld параметр (%s:%d)\n\n", index_of_parameter, __FILE__, __LINE__);

// 		index_of_parameter += 1; 

// 		status = write_identifier_node (node -> right, tree_in_asm);
// 		if (status) {return status;}

// 		node = node -> left;
// 	}

// 	tree_in_asm -> position_in_assign = NOT_IN_ASSIGN;

// 	return NOT_ERROR;
// }

static language_error_t write_return_in_asm (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";завершение функции  (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";вычисляет значение, возвращаемое функцией  (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_node_in_asm (node -> right, tree_in_asm);
	if (status) {return status;}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "pop rax    ;записывает возвращаемое функцией значение  (%s:%d)\n\n", __FILE__, __LINE__);

	if (tree_in_asm -> status_of_func != MAIN_FUNC)
	{
		print_tabulation_in_file (tree_in_asm);
		fprintf (file_for_asm, "ret    ;переход по метке к месту вызова функции   (%s:%d)\n\n", __FILE__, __LINE__);
	}

	return NOT_ERROR;
}

static language_error_t write_call_node (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	language_error_t status = NOT_ERROR;

	size_t index_id_in_name_table = node -> right -> value.value_identifier.index_id_in_name_table;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";начинает вызов функции   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "mov r15, rsp  ;сохраняем rsp   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "sub rsp, 8  ;место под rip от call   (%s:%d)\n\n", __FILE__, __LINE__);


	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "push r12    ;сохраняет значение старого стекового фрейма  (%s:%d)\n\n", __FILE__, __LINE__);


	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";записывает аргументы для функции (%s:%d)\n\n", __FILE__, __LINE__);

	status = write_parameters_for_call (node -> left, tree_in_asm);
	if (status) {return status;}



	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "mov r12, rsp    ;(%s:%d)\n\n", __FILE__, __LINE__);

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, ";записывает аргументы для функции (%s:%d)\n\n", __FILE__, __LINE__);

	// status = write_parameters_for_call (node -> left, tree_in_asm);
	// if (status) {return status;}






	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, ";смещение начала локальной области видимости: изменение стекового фрейма (%s:%d)\n\n", __FILE__, __LINE__);

	// size_t index_local_table_in_list = find_position_of_local_table (tree_in_asm -> list_of_local_name_tables, index_id_in_name_table);

	// local_name_table_t* array_of_local_name_table    = tree_in_asm -> list_of_local_name_tables -> array_of_local_name_table;
	// size_t              count_local_arg_in_call_func = array_of_local_name_table[index_local_table_in_list].free_index_in_local_name_table;

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "sub r12, %ld * 8    ;(%s:%d)\n\n", count_local_arg_in_call_func, __FILE__, __LINE__);
	








	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "add r12, 8    ;(%s:%d)\n\n", __FILE__, __LINE__);



	
	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "push BX   #начало старого стекового фрейма\n\n");

			

			// print_tabulation_in_file (tree_in_asm);
			// fprintf (file_for_asm, "sub r12, %ld * 8   ;смещение начала локальной области видимости   (%s:%d)\n\n", count_local_arg_in_call_func, __FILE__, __LINE__);

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "push %ld    #записывает количество локальных переменных в функции\n\n", 
	//             (tree_in_asm -> list_of_local_name_tables -> array_of_local_name_table)[index_local_table_in_list].free_index_in_local_name_table);

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "add    #начало нового стекового фрейма\n\n");

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "pop BX    #сохраняет новое старого стекового фрейма\n\n");

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";вызывает функцию   (%s:%d)\n\n", __FILE__, __LINE__);
	
	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "mov rsp, r15    ;(%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "call ");

	// status = write_identifier_node (node -> right, tree_in_asm);
	// if (status) return status;

	name_t name_in_node = (tree_in_asm -> name_table -> array_names)[index_id_in_name_table];

	print_symbols_from_str_in_file (tree_in_asm -> file_for_asm, 
		                            tree_in_asm -> str_with_table + name_in_node.index_to_name_in_str,
																	name_in_node.len_name);

	fprintf (file_for_asm, "    ;вызов функции   (%s:%d)\n\n", __FILE__, __LINE__);


	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "add rsp, %ld * 8    ;  (%s:%d)\n\n", count_local_arg_in_call_func, __FILE__, __LINE__);

	// print_tabulation_in_file (tree_in_asm);
	// fprintf (file_for_asm, "pop r12    ;возвращает предыдущее значение начала стекового фрейма   (%s:%d)\n\n", __FILE__, __LINE__);

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "push rax    ;кладёт в стек значение, которое вернула функция    (%s:%d)\n\n", __FILE__, __LINE__);	

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, ";конец вызова функции    (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

static language_error_t write_parameters_for_call (node_t* node, tree_in_asm_t* tree_in_asm)
{
	assert (node);
	assert (tree_in_asm);

	language_error_t status = NOT_ERROR;

	FILE* file_for_asm = tree_in_asm -> file_for_asm;

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "; начало записи значений аргументов, передаваемых функции (%s:%d)\n\n", __FILE__, __LINE__);

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
		fprintf (file_for_asm, "; %ld аргумент   (%s:%d)\n\n", index, __FILE__, __LINE__);

		status = write_node_in_asm (node -> right, tree_in_asm);
		if (status) {return status;}

		node = node -> parent;
	}

	print_tabulation_in_file (tree_in_asm);
	fprintf (file_for_asm, "; конец записи значений аргументов, передаваемых функции  (%s:%d)\n\n", __FILE__, __LINE__);

	return NOT_ERROR;
}

