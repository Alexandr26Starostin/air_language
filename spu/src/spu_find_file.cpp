#include <stdio.h>
#include <assert.h>

#include "spu_find_file.h"

//-----------------------------------------------------------------------

result_find_file find_file (int argc, char** argv, FILE** ptr_cmd_file)
{
	assert (argv);	
	assert (ptr_cmd_file);

	if (argc >= 2)
	{	
		*ptr_cmd_file = fopen (argv[1], "rb");
	}

	else 
	{
		printf ("spu wants to eat one files)) Don't be greedy\n");
		return FEW_FILES;
	}

	if (*ptr_cmd_file == NULL)
	{
		printf ("cmd_file == NULL\n");

		printf ("Not find file\n"); 
		printf ("Write in format: 1) exe  2) commands  3) other \n");

		fclose (*ptr_cmd_file);

		return NOT_FIND_FILE;
	}

	return YES_FIND_FILE;
}