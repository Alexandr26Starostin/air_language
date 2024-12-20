#ifndef COMMANDS_H
#define COMMANDS_H

#define DEF_CMD_(name_cmd, num, ...) \
	cmd_##name_cmd = num,

enum command_t
{
	#include "dsl.cpp"
};

#undef DEF_CMD_

#endif