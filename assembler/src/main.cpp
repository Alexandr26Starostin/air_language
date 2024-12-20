#include <stdio.h>
#include <assert.h>

#include "asm_find_file.h"
#include "launch_asm.h"

//-----------------------------------------------------------------------------

int main (int argc, char** argv)
{
	FILE* asm_file = NULL; 
	FILE* cmd_file = NULL;

	long status_find_file = find_file (argc, argv, &asm_file, &cmd_file);
	if (status_find_file) {return (int) status_find_file;}

	long status_launch_asm = launch_asm (asm_file, cmd_file);
	if (status_launch_asm) {return (int) status_launch_asm;}

	return (int) status_launch_asm;
}
