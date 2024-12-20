#ifndef ASM_LABELS_H
#define ASM_LABELS_H

#include "launch_asm.h"

void write_in_labels (asm_t* ptr_assm, char* label);
long find_label      (asm_t* ptr_assm, char* label);
void write_in_fix_up (asm_t* ptr_assm);
void read_fix_up     (asm_t* ptr_assm);

#endif
