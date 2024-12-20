#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "commands.h"
#include "launch_asm.h"
#include "asm_error.h"
#include "asm_print.h"
#include "asm_translate.h"
#include "asm_labels.h"

static long read_file     (FILE*  asm_file, char** ptr_text);
static void write_in_file (asm_t* ptr_assm, FILE* code_file);
static long len_file      (FILE*  file);  
static void close_files   (FILE*  asm_file, FILE* cmd_file);
static void creat_asm     (asm_t* ptr_assm);
static void detroy_asm    (asm_t* ptr_assm);

#define DESTROY_ASM_(asm_file, cmd_file, assm, text) \
	close_files (asm_file, cmd_file); \
	detroy_asm  (&assm); \
	free (text);

//-----------------------------------------------------------------------------------------------------------------------------------------------

long launch_asm (FILE* asm_file, FILE* cmd_file)
{
	assert (asm_file);
	assert (cmd_file);

	char* text             = NULL;
	long  status_read_file = NOT_ERROR;

	status_read_file = read_file (asm_file, &text);

	if (status_read_file)
	{
		printf ("%s:%d ERROR in read_file\n", __FILE__, __LINE__);
		close_files (asm_file, cmd_file);
		return status_read_file; 
	}

	asm_t assm = {};
	creat_asm (&assm);

	if (asm_error (&assm, __FILE__, __LINE__))
	{
		DESTROY_ASM_(asm_file, cmd_file, assm, text)

		return assm.error_in_asm;
	}

	if (translate_asm (&assm, text))
	{
		DESTROY_ASM_(asm_file, cmd_file, assm, text)

		return assm.error_in_asm;
	}

	read_fix_up (&assm);

	if (asm_error (&assm, __FILE__, __LINE__))
	{
		DESTROY_ASM_(asm_file, cmd_file, assm, text)

		return assm.error_in_asm;
	}

	write_in_file (&assm, cmd_file);

	DESTROY_ASM_(asm_file, cmd_file, assm, text)

	return NOT_ERROR;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

static long read_file (FILE* asm_file, char** ptr_text)
{
	assert (asm_file);
	assert (ptr_text);

    long count_memory = len_file (asm_file);

	*ptr_text = (char*) calloc (count_memory, sizeof (char));
	if (*ptr_text == NULL) 
	{
		printf ("%s:%d  *text == NULL\n", __FILE__, __LINE__);
		return (long) PTR_TEXT_NULL; 
	}

	long count_fread = (long) fread (*ptr_text, sizeof (char), count_memory, asm_file);
	if (count_fread != count_memory)
	{
		#ifdef PRINT_ASM_
			printf ("count_fread  != count_memory\n");
			printf ("count_fread  == %ld\n", count_fread);
			printf ("count_memory == %ld\n", count_memory);
		#endif

		return (long) ERROR_IN_FREAD;
	}

	return (long) NOT_ERROR;
}

static void write_in_file (asm_t* ptr_assm, FILE* code_file)
{
	assert (ptr_assm);
	assert (code_file);

	int hdr[SIZE_HEADER] = {};
	hdr[0] = SIGNATURE;
	hdr[1] = VERSION;
	hdr[2] = (int) ptr_assm -> cmd_count;

	fwrite  (hdr,             sizeof (int),  SIZE_HEADER,           code_file);
	fwrite  (ptr_assm -> cmd, sizeof (long), ptr_assm -> cmd_count, code_file);
}

static long len_file (FILE* file)      
{ 
    assert (file);

    fseek (file, 0, SEEK_END);
    const long count_memory = ftell (file);
    fseek (file, 0, SEEK_SET);

    return count_memory;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

static void creat_asm (asm_t* ptr_assm)
{
 	assert (ptr_assm);

	ptr_assm -> error_in_asm = NOT_ERROR;

	ptr_assm -> cmd_count    = 0;
	ptr_assm -> fix_up_count = 0;
	ptr_assm -> labels_count = 0;

	ptr_assm -> cmd    = (int*)     calloc  (SIZE_CMD,    sizeof  (int));
	ptr_assm -> fix_up = (fix_t*)   calloc (SIZE_FIX_UP, sizeof (fix_t));
	ptr_assm -> labels = (label_t*) calloc (SIZE_LABELS, sizeof (label_t));
}

static void detroy_asm (asm_t* ptr_assm)
{
	assert (ptr_assm);

	free (ptr_assm -> cmd);
	free (ptr_assm -> fix_up);
	free (ptr_assm -> labels);
}

static void close_files (FILE* cmd_file, FILE* code_file)
{
	assert (cmd_file);
	assert (code_file);

	fclose (cmd_file);
	fclose (code_file);
}
