#ifndef FIND_FILE_H
#define FIND_FILE_H

enum result_find_file: long
{
	FEW_FILES     = -2,
	NOT_FIND_FILE = -1,
	YES_FIND_FILE = 0
};

#include <stdio.h>

result_find_file find_file (int argc, char** argv, FILE** ptr_asm_file, FILE** ptr_cmd_file);

#endif
