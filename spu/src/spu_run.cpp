#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "const_define_struct.h"
#include "hash.h"
#include "operations_with_stack.h"
#include "stk_error.h"

#include "commands.h"
#include "spu_error.h"
#include "spu_print.h"
#include "spu_run.h"
#include "spu_draw_picture.h"

const int   POISON          = -66666;
const char* NAME_GUIDE_FILE = "../guide.txt";

static int  get_arg_push (spu_t* ptr_spu, size_t* ip);
static int* get_arg_pop  (spu_t* ptr_spu, size_t* ip);

#define DEF_CMD_(name_cmd, num, arg, ...)    \
	case cmd_##name_cmd: {__VA_ARGS__; break;}

//--------------------------------------------------------------------------------------------------------------------------

long run_spu (spu_t* ptr_spu)
{
	assert (ptr_spu);

	FILE* guide_file = fopen (NAME_GUIDE_FILE, "r");
	if (guide_file == NULL) 
	{
		printf ("Can't open guide.txt\n"); 
		ptr_spu -> error_in_spu |= NOT_FIND_GUIDE;
	}

	size_t ip = 0;

	for (ip = 0; ip < ptr_spu -> size_cmd; ip++)
	{
		if (spu_error (ptr_spu, __FILE__, __LINE__))
		{
			fclose (guide_file);
			return ptr_spu -> error_in_spu;
		}
		#ifdef PRINT_SPU_
			print_spu (ptr_spu, ip);
		#endif

		cmd_t command = (ptr_spu -> cmd)[ip];

		

		switch (command & COMMAND_MACK)
		{
			#include "dsl.cpp"
			
			default:
			{
				printf ("SNT_ERROR: '%x'\n", command);
				fclose      (guide_file);

				ptr_spu -> error_in_spu |= INCORRECT_COMMAND;

				spu_error (ptr_spu, __FILE__, __LINE__);
				
				return ptr_spu -> error_in_spu;

				break;
			}
		}
	}

	if (spu_error (ptr_spu, __FILE__, __LINE__))
	{
		fclose (guide_file);
		return ptr_spu -> error_in_spu;
	}
	
	#ifdef PRINT_SPU_
		printf ("in end:\n");
		print_spu (ptr_spu, ip);
	#endif

	return ptr_spu -> error_in_spu;
}

//-------------------------------------------------------------------------------------------------

static int get_arg_push (spu_t* ptr_spu, size_t* ip)
{
	assert (ptr_spu);
	assert (ip);

	cmd_t command = (ptr_spu -> cmd) [(*ip)++];
	int   arg     = 0;

	if (command & IMM_MASK) 
	{
		arg = (ptr_spu -> cmd) [(*ip)++];
	}

	if (command & REG_MASK) 
	{
		int index_reg = (ptr_spu -> cmd) [(*ip)++];

		if (index_reg >= (int) SIZE_REG || index_reg < 0)
		{
			ptr_spu -> error_in_spu |= SIZE_REG_EXCEED;
			return POISON;
		}

		arg += (ptr_spu -> reg)[index_reg];
	}

	if (command & RAM_MASK)
	{
		if (arg >= (int) SIZE_RAM)
		{
			ptr_spu -> error_in_spu |= SIZE_RAM_EXCEED;
			return POISON;
		}

		arg = (ptr_spu -> ram)[arg];
	}

	(*ip) -= 1;

	return arg;
}

static int* get_arg_pop (spu_t* ptr_spu, size_t* ip)
{
	assert (ptr_spu);
	assert (ip);

	

	cmd_t command     = (ptr_spu -> cmd) [(*ip)++];
	int*  ptr_for_arg = NULL;
	int   arg         = 0;

	if (command & IMM_MASK) 
	{
		arg = (ptr_spu -> cmd) [(*ip)++];
	}

	if (command & REG_MASK) 
	{
		int index_reg = (ptr_spu -> cmd) [(*ip)++];

		if (index_reg >= (int) SIZE_REG || index_reg < 0)
		{
			ptr_spu -> error_in_spu |= SIZE_REG_EXCEED;
			return NULL;
		}

		ptr_for_arg = ((ptr_spu -> reg) + index_reg);
	}

	if (command & RAM_MASK)
	{
		if (arg >= (int) SIZE_RAM)
		{
			ptr_spu -> error_in_spu |= SIZE_RAM_EXCEED;
			return NULL;
		}

		if (ptr_for_arg != NULL)
			ptr_for_arg = ((ptr_spu -> ram) + *ptr_for_arg + arg);
		
		else
			ptr_for_arg = ((ptr_spu -> ram) + arg);
	}

	(*ip) -= 1;

	return ptr_for_arg;
}


#undef DEF_CMD_