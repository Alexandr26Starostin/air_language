#ifndef SPU_ERROR_H
#define SPU_ERROR_H

#include "launch_spu.h"

long spu_error   (spu_t* ptr_spu, const char* file, int line);
long verifier 	 (spu_t* ptr_spu, const char* file, int line);
void print_error (long   danger_bit);

#endif