#ifndef FIND_FILE_H
#define FIND_FILE_H

#include <stdio.h>

enum result_find_file
{
	FEW_FILES     = -2,
	NOT_FIND_FILE = -1,
	YES_FIND_FILE = 0
};

result_find_file find_file (int argc, char** argv, FILE** ptr_cmd_file);

#endif