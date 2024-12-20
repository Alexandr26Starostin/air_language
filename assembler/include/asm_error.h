#ifndef ASM_ERROR_H
#define ASM_ERROR_H

#include "launch_asm.h"

long asm_error   (asm_t* ptr_assm, const char* file, int line);
long verifier 	 (asm_t* ptr_assm, const char* file, int line);
void print_error (long   danger_bit);

#endif
