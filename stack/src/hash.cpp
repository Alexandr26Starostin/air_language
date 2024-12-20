#include <stdio.h>
#include <assert.h>

#include "../include/const_define_struct.h"
#include "../include/hash.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef HASH_STK

	hash_t hash (char* ptr_stk, size_t len)
	{
		assert (ptr_stk);

		hash_t hash = number_hash;

		for (size_t byte = 0; byte < len; byte++)
		{
			hash = ((hash << 5) + hash) ^ ptr_stk [byte];
		}

		return hash;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------

	errors_t count_hash (stk_t* ptr_stk)
	{
		assert (ptr_stk);

		COUNT_HASH;

		(*ptr_stk).hash_stk      = hash_1;
		(*ptr_stk).hash_stk_data = hash_2;

		return NOT_ERROR;
	}

#endif