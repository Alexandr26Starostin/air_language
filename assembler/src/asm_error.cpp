#include <stdio.h>
#include <assert.h>

#include "commands.h"
#include "launch_asm.h"
#include "asm_error.h"

#define CHECK_ASSM_ON_NULL_(ptr_array, name_error)   \
	if (ptr_assm -> ptr_array == NULL)               \
	{                                                \
		ptr_assm -> error_in_asm |= name_error;      \
 	}                           

#define CHECK_ASSM_ON_SIZE_(size_array, const_size_array, name_error)  \
	if (ptr_assm -> size_array >= const_size_array)                    \
	{												                   \
		ptr_assm -> error_in_asm |= name_error;                        \
	}

//------------------------------------------------------------------------------------------------------------------------------------------

long asm_error (asm_t* ptr_assm, const char* file, int line)
{
	assert (ptr_assm);

	CHECK_ASSM_ON_NULL_(cmd,    CMD_NULL);
	CHECK_ASSM_ON_NULL_(fix_up, FIX_UP_NULL);
	CHECK_ASSM_ON_NULL_(labels, LABELS_NULL);

	CHECK_ASSM_ON_SIZE_(cmd_count,    SIZE_CMD,    SIZE_CMD_EXCEED);
	CHECK_ASSM_ON_SIZE_(labels_count, SIZE_LABELS, SIZE_LABELS_EXCEED);
	CHECK_ASSM_ON_SIZE_(fix_up_count, SIZE_FIX_UP, SIZE_FIX_UP_EXCEED);

	long status = ptr_assm -> error_in_asm;

	if (status)
	{
		verifier (ptr_assm, file, line);
	}

	return status;
}

long verifier (asm_t* ptr_assm, const char* file, int line)
{
	assert (ptr_assm);
	assert (file);
	long status = ptr_assm -> error_in_asm;

	if (status)
	{
		printf ("Find error in %s:%d\n", file, line);

		for (size_t index_bit = 0; index_bit < MAX_BIT; index_bit++)
		{
			long danger_bit = (1 << index_bit) & status;
			if (danger_bit)
			{
				print_error (danger_bit);
			}
		}
	}
	return status;
}

void print_error (long danger_bit)
{
	switch (danger_bit)
	{
		case CMD_NULL:           	 {printf ("pointer on cmd    == NULL;                                     code_error == %ld\n", danger_bit);     break;}
		case FIX_UP_NULL:        	 {printf ("pointer on fix_up == NULL;                                     code_error == %ld\n", danger_bit);     break;}
		case LABELS_NULL:        	 {printf ("pointer on labels == NULL;                                     code_error == %ld\n", danger_bit);     break;}
		case SIZE_CMD_EXCEED:    	 {printf ("exceed the size of cmd;    change the max size of cmd;         code_error == %ld\n", danger_bit);     break;}
		case SIZE_LABELS_EXCEED: 	 {printf ("exceed the size of labels; change the max size of labels;      code_error == %ld\n", danger_bit);     break;}
		case SIZE_FIX_UP_EXCEED: 	 {printf ("exceed the size of fix_up; change the max size of fix_up;      code_error == %ld\n", danger_bit);     break;}
		case PUSH_INCORRECT:     	 {printf ("push incorrect; use 'print_asm' to find, where error was born; code_error == %ld\n", danger_bit);     break;}
		case POP_INCORRECT:      	 {printf ("pop  incorrect; use 'print_asm' to find, where error was born; code_error == %ld\n", danger_bit);     break;}
		case COMMAND_INCORRECT:  	 {printf ("command incorrect;                                             code_error == %ld\n", danger_bit);     break;}
		case REPEATED_LABEL:     	 {printf ("label defined in two places;                                   code_error == %ld\n", danger_bit);     break;}
		case LABEL_INCORRECT:        {printf ("not find ':' in label;                                         code_error == %ld\n", danger_bit);     break;}
		case NO_DEFINITION_OF_LABEL: {printf ("not definition of label;                                       code_error == %ld\n", danger_bit);     break;}
		default:                 	 {printf ("this error not find: %ld\n",                                                         danger_bit);     break;}
	}
}