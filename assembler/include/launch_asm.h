#ifndef LAUNCH_ASM_H
#define LAUNCH_ASM_H

#define PRINT_ASM_NOT
#define PRINT_CMD_
#define PRINT_LABELS_
#define PRINT_FIX_UP_

const size_t MAX_LEN_STR   = 50;
const size_t MAX_LEN_LABEL = 50; 

const size_t SIZE_CMD      = 526;
const size_t SIZE_LABELS   = 128;
const size_t SIZE_FIX_UP   = 128;
const size_t MAX_BIT       = sizeof (long) * 8;

const int SIZE_HEADER   = 3;
const int SIGNATURE     = 0xC0FFEE;
const int VERSION       = 1;  

const long   IMM_MASK    = 0x20;
const long   REG_MASK    = 0x40;
const long   MEM_MASK    = 0x80;

enum error_t
{
	ERROR_IN_HEADER        = -5,
	PTR_TEXT_NULL          = -4,
	ERROR_IN_FREAD         = -3,
	NOT_ERROR              = 0,
	CMD_NULL               = 1,
	FIX_UP_NULL            = 2,
	LABELS_NULL            = 4,
	SIZE_CMD_EXCEED        = 8,
	SIZE_LABELS_EXCEED     = 16,
	SIZE_FIX_UP_EXCEED     = 32,
	PUSH_INCORRECT         = 64,
	POP_INCORRECT          = 128,
	COMMAND_INCORRECT      = 256,
	REPEATED_LABEL         = 512,
	LABEL_INCORRECT        = 1024,
	NO_DEFINITION_OF_LABEL = 2048
};

struct fix_t
{
	size_t cmd_fix;
	size_t label_fix;
};

struct label_t 
{
	int   ip_label;
	char  name_label [MAX_LEN_LABEL] = "";
};

struct asm_t
{
	long     error_in_asm;
	int*     cmd;
	fix_t*   fix_up;
	label_t* labels;
	size_t   cmd_count;
	size_t   fix_up_count;
	size_t   labels_count;
};

long launch_asm (FILE* asm_file, FILE* cmd_file);

#endif
