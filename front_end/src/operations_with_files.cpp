#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "const_in_front_end.h"
#include "operations_with_files.h"

static FILE*  find_program_file (int argc, char** argv);
static size_t count_len_file    (FILE* file);

//---------------------------------------------------------------------------------------------------------

front_end_error_t read_program_file (int argc, char** argv, char** ptr_str_with_program)
{
	assert (argv);
	assert (ptr_str_with_program);

	FILE* program_file = find_program_file (argc, argv);
	if (program_file == NULL) {return NOT_FIND_FILE_WITH_PROGRAM;}

	size_t len_file = count_len_file (program_file);

	*ptr_str_with_program = (char*) calloc (len_file, sizeof (char));
	if (*ptr_str_with_program == NULL)
	{
		printf ("\n\nError in %s:%d\n"
		        "not memory for str_with_program\n\n", __FILE__, __LINE__);

		fclose (program_file);

		return NOT_MEMORY_FOR_STR_WITH_PROGRAM;
	}

	fread (*ptr_str_with_program, sizeof (char), len_file, program_file);

	fclose (program_file);

	return NOT_ERROR;
}

static FILE* find_program_file (int argc, char** argv)
{
	assert (argv);

	if (argc < POSITION_NAME_FILE_IN_ARGV)
	{
		printf ("\n\nError in %s:%d\n"
		        "front_end not give file with program\n"
		        "Example of using front_end:\n"
		        "./front_end <name_program> <flags>\n\n", __FILE__, __LINE__);

		return NULL;
	}

	char* name_program_file = argv[POSITION_NAME_FILE_IN_ARGV];

	FILE* program_file = fopen (name_program_file, "r");
	if (program_file == NULL)
	{
		printf ("\n\nError in %s:%d\n"
		        "front_end not find file with program, which have name: \"%s\"\n"
		        "Example of using front_end:\n"
		        "./front_end <name_program> <flags>\n\n", __FILE__, __LINE__, name_program_file);

		return NULL;
	}

	return program_file;
}

static size_t count_len_file (FILE* file)      
{ 
    assert (file);

    fseek (file, 0, SEEK_END);
    size_t count_memory = ftell (file);
    fseek (file, 0, SEEK_SET);

    return count_memory;
}