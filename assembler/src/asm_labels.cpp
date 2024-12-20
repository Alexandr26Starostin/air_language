#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "launch_asm.h"
#include "asm_labels.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

void write_in_labels (asm_t* ptr_assm, char* label)
{
	assert (ptr_assm);
	assert (label);

	size_t index_label = 0;

	for (index_label = 0; index_label < (ptr_assm -> labels_count); index_label++)
	{
		if (strcmp (label, ((ptr_assm -> labels) [index_label]).name_label) == 0)
		{
			if (((ptr_assm -> labels) [index_label]).ip_label != -1)
			{
				ptr_assm -> error_in_asm |= REPEATED_LABEL;
			}

			break;
		}
	}

	((ptr_assm -> labels) [index_label]).ip_label   = (int) ptr_assm -> cmd_count;
	strcpy(((ptr_assm -> labels) [index_label]).name_label, label);

	if (index_label == (ptr_assm -> labels_count))
	{
		(ptr_assm -> labels_count) += 1;
	}
}

long find_label (asm_t* ptr_assm, char* label)
{
	assert (ptr_assm);
	assert (label);

	for (size_t index_label = 0; index_label < ptr_assm -> labels_count; index_label++)
	{
		if (strcmp (label, ((ptr_assm -> labels) [index_label]).name_label) == 0)
		{
			return ((ptr_assm -> labels) [index_label]).ip_label;
		}
	}

	((ptr_assm -> labels) [ptr_assm -> labels_count]).ip_label = -1;
	strcpy(((ptr_assm -> labels) [ptr_assm -> labels_count]).name_label, label);

	(ptr_assm -> labels_count) += 1;

	return -1;
}

void write_in_fix_up (asm_t* ptr_assm)
{
	assert (ptr_assm);

	((ptr_assm -> fix_up) [ptr_assm -> fix_up_count]).cmd_fix   = ptr_assm -> cmd_count;
	((ptr_assm -> fix_up) [ptr_assm -> fix_up_count]).label_fix = (ptr_assm -> labels_count) - 1;

	ptr_assm -> fix_up_count += 1;
}

void read_fix_up (asm_t* ptr_assm)
{
	assert (ptr_assm);

	for (size_t index_fix_up = 0; index_fix_up < ptr_assm -> fix_up_count; index_fix_up++)
	{
		size_t index_true_label = ((ptr_assm -> fix_up) [index_fix_up]).label_fix;
		int    true_ip_label    = ((ptr_assm -> labels) [index_true_label]).ip_label;

		if (true_ip_label == -1)
		{
			ptr_assm -> error_in_asm |= NO_DEFINITION_OF_LABEL;

			printf ("not define the label: %s\n", ((ptr_assm -> labels)[index_true_label]).name_label);
			break;
		}

		size_t index_incorrect_cmd = ((ptr_assm -> fix_up) [index_fix_up]).cmd_fix;

		(ptr_assm -> cmd) [index_incorrect_cmd] = true_ip_label;
	}
}
