#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "asm_find_file.h"

result_find_file find_file (int argc, char** argv, FILE** ptr_asm_file, FILE** ptr_cmd_file)
{
	assert (argv);	
	assert (ptr_asm_file);
	assert (ptr_cmd_file);

	if (argc >= 3)
	{
		*ptr_asm_file = fopen (argv[1], "rb");
		*ptr_cmd_file = fopen (argv[2], "wb");
	}

	else 
	{
		printf ("assembler wants to eat two files)) Don't be greedy\n");
		return FEW_FILES;
	}

	if (*ptr_asm_file == NULL || *ptr_cmd_file == NULL)
	{
		if (*ptr_asm_file == NULL) {printf ("asm_file == NULL\n");}
		if (*ptr_cmd_file == NULL) {printf ("cmd_file == NULL\n");}

		printf ("Not find files\n"); 
		printf ("Write in format: 1) exe  2) commands  3) code  4) other \n");

		fclose (*ptr_asm_file);
		fclose (*ptr_cmd_file);

		return NOT_FIND_FILE;
	}

	return YES_FIND_FILE;
}