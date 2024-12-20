#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "commands.h"
#include "launch_asm.h"
#include "asm_error.h"
#include "asm_print.h"
#include "asm_translate.h"
#include "asm_labels.h"

const int POISON = -666666;

static long put_arg_push (char** ptr_text, char* word_in_text, int* index_text, asm_t* ptr_assm);
static long put_arg_pop  (char** ptr_text, char* word_in_text, int* index_text, asm_t* ptr_assm);
static long compile_arg  (char** ptr_text, char* word_in_text, int* index_text, asm_t* ptr_assm, command_t cmd);

#define DEF_CMD_(name_cmd, num, arg, ...)  \
	if (strcmp (word_in_text, #name_cmd) == 0)  \
	{                                           \
		if (arg) {compile_arg (&ptr_text, word_in_text, &index_text, ptr_assm, cmd_##name_cmd);}	                                      \
		else {(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = cmd_##name_cmd;}                            \
		continue;                                                                                     \
	}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------

long translate_asm (asm_t* ptr_assm, char* text)
{
	assert (ptr_assm);
	assert (text);

	char  word_in_text[MAX_LEN_STR] = "";
	int   index_text                = 0;
	char* ptr_text                  = text; 
	
	while (sscanf (ptr_text, "%s%n", word_in_text, &index_text) != EOF)
	{
		if (asm_error (ptr_assm, __FILE__, __LINE__))
		{
			return ptr_assm -> error_in_asm;
		}

		#ifdef PRINT_ASM_
			printf ("word from sscanf: %s\n", word_in_text);
		#endif

		#ifdef PRINT_ASM_
			print_asm (ptr_assm);
		#endif

		ptr_text += index_text;

		#include "dsl.cpp"

		if (word_in_text[0] == '#') 
		{
			ptr_text = strchr(ptr_text, '\n') + 1;
			continue;
		}

		if (strchr(word_in_text, ':') != NULL)
		{
			write_in_labels (ptr_assm, word_in_text);
			continue;
		}

		printf ("SNT_ERROR:\n command: '%s' don't find\n", word_in_text);

		ptr_assm -> error_in_asm |= COMMAND_INCORRECT;
	}

	if (asm_error (ptr_assm, __FILE__, __LINE__))
	{
		return ptr_assm -> error_in_asm;
	}

	#ifdef PRINT_ASM_
		print_asm (ptr_assm);
	#endif

	return ptr_assm -> error_in_asm;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

static long put_arg_push (char** ptr_text, char* word_in_text, int* index_text, asm_t* ptr_assm)
{
	assert (ptr_text);
	assert (*ptr_text);
	assert (word_in_text);
	assert (index_text);
	assert (ptr_assm);

	sscanf (*ptr_text, "%s%n", word_in_text, index_text);
	*ptr_text += *index_text;

	int put_push  = cmd_push;
	int index_reg = 0;
	int arg       = POISON;


	if ((word_in_text[0] == '[') && (strchr (word_in_text, ']') != NULL))
	{
		put_push |= MEM_MASK;
	}

	char* ptr_on_PLUS = strchr (word_in_text, '+');
	char* ptr_on_X    = strchr (word_in_text, 'X');

	if (ptr_on_PLUS != NULL)
			{
				put_push |= (REG_MASK | IMM_MASK);
			}

			if (ptr_on_X != NULL)
			{
				if (ptr_on_PLUS == NULL)
				{
					put_push |= REG_MASK;
				}

				index_reg = (int) *(ptr_on_X - 1) - 'A' + 1;
			}

			if (ptr_on_PLUS != NULL)
			{
				ptr_on_PLUS += 1;
				//while (ptr_on_PLUS[0] ==' ') {ptr_on_PLUS += 1;}

				sscanf (ptr_on_PLUS, "%d", &arg);
			}

			if ((put_push & (REG_MASK | IMM_MASK)) == 0)
			{
				size_t index_latter = 0;
				
				while (word_in_text [index_latter] != '\0')
				{
					if (isdigit(word_in_text [index_latter]))
					{
						put_push |= IMM_MASK;
						sscanf (word_in_text + index_latter, "%d", &arg);
						
						break;
					}

					index_latter++;
				}
			}

			if ((put_push & (REG_MASK | IMM_MASK)) == 0)
			{
				ptr_assm -> error_in_asm |= PUSH_INCORRECT;
				return ptr_assm -> error_in_asm;
			}

			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_push;

			if (arg != POISON)
			{
				if (strchr (word_in_text, '-') != NULL)
				{
					arg *= -1;
				}

				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
			}

			if (index_reg > 0)
			{
				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = index_reg;
			}

			return ptr_assm -> error_in_asm;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

static long put_arg_pop  (char** ptr_text, char* word_in_text, int* index_text, asm_t* ptr_assm)
{
	assert (ptr_text);
	assert (*ptr_text);
	assert (word_in_text);
	assert (index_text);
	assert (ptr_assm);

	sscanf (*ptr_text, "%s%n", word_in_text, index_text);
	*ptr_text += *index_text;

	int put_pop   = cmd_pop;
	int index_reg = 0;
	int arg       = POISON;

	if ((word_in_text[0] == '[') && (strchr (word_in_text, ']') != NULL))
	{
		put_pop |= MEM_MASK;
	}

	char* ptr_on_PLUS = strchr (word_in_text, '+');
	char* ptr_on_X    = strchr (word_in_text, 'X');

	if (ptr_on_PLUS != NULL && ((put_pop & MEM_MASK) == 0))
	{
		ptr_assm -> error_in_asm |= POP_INCORRECT;
		return ptr_assm -> error_in_asm;
	}

	if (ptr_on_PLUS != NULL)
	{
		put_pop |= (REG_MASK | IMM_MASK);
	}

	if (ptr_on_X != NULL)
	{
		if (ptr_on_PLUS == NULL)
		{
			put_pop |= REG_MASK;
		}

		index_reg = (int) *(ptr_on_X - 1) - 'A' + 1;
	}

	if (ptr_on_PLUS != NULL)
	{
		ptr_on_PLUS += 1;
		//while (ptr_on_PLUS[0] ==' ') {ptr_on_PLUS += 1;}

		sscanf (ptr_on_PLUS, "%d", &arg);
	}

	if ((put_pop & (REG_MASK | IMM_MASK)) == 0 && (put_pop & MEM_MASK) != 0)
	{
		size_t index_latter = 0;
				
		while (word_in_text [index_latter] != '\0')
		{
			if (isdigit(word_in_text [index_latter]))
			{
				put_pop |= IMM_MASK;

				sscanf (word_in_text + index_latter, "%d", &arg);
						
				break;
			}

			index_latter++;
		}
	}

	if ((put_pop & (REG_MASK | IMM_MASK)) == 0)
	{
		ptr_assm -> error_in_asm |= POP_INCORRECT;
		return ptr_assm -> error_in_asm;
	}

	(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_pop;

	if (arg != POISON)
	{
		(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
	}

	if (index_reg > 0)
	{
		(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = index_reg;
	}

	return ptr_assm -> error_in_asm;
}

static long compile_arg  (char** ptr_text, char* word_in_text, int* index_text, asm_t* ptr_assm, command_t name_cmd)
{
	assert (ptr_text);
	assert (*ptr_text);
	assert (word_in_text);
	assert (index_text);
	assert (ptr_assm);

	if (name_cmd == cmd_pop)
	{
		put_arg_pop (ptr_text, word_in_text, index_text, ptr_assm);
		return ptr_assm -> error_in_asm;
	}

	if (name_cmd == cmd_push)
	{
		put_arg_push (ptr_text, word_in_text, index_text, ptr_assm);
		return ptr_assm -> error_in_asm;
	}

	(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = name_cmd;

	if (name_cmd == cmd_draw)
	{
		int arg = 0;
		sscanf (*ptr_text, "%d%n", &arg, index_text);
		*ptr_text += *index_text;

		(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;

		return ptr_assm -> error_in_asm;
	}

	//jump:

	sscanf (*ptr_text, "%s%n", word_in_text, index_text);        
                                                                      
	*ptr_text += *index_text;                                       
                                                                      
	if (strchr(word_in_text, ':') != NULL)                        
	{                                                             
		long label = find_label (ptr_assm, word_in_text);         
		(ptr_assm -> cmd) [ptr_assm -> cmd_count] = (int) label;  
		if (label < 0)                                            
		{                                                         
			write_in_fix_up (ptr_assm);                           
		}                                                         
                                                                      
		(ptr_assm -> cmd_count) += 1;                             
	}                                                             
                                                                      
	else                                                          
	{                                                             
		ptr_assm -> error_in_asm |= LABEL_INCORRECT;              
	}                                                             
                                                                      
	return ptr_assm -> error_in_asm;                                                     
}

#undef DEF_CMD_

