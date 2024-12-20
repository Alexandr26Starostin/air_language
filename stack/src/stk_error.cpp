#include <stdio.h>
#include <assert.h>

#include "../include/const_define_struct.h"
#include "../include/hash.h"
#include "../include/stk_error.h"

 //----------------------------------------------------------------------------------------------------------------------------------------------------------------

errors_t stk_error (stk_t* ptr_stk)
{
	assert (ptr_stk);
    
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------

	if ((*ptr_stk).data == NULL)
	{
		return FALL_PTR_DATA;
	}
    
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	if ((*ptr_stk).size > (*ptr_stk).capacity)
	{
		return SIZE_MORE_CAPACITY;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	#ifdef CANARY_STK
		if (((*ptr_stk).canary_1 != canary) || ((*ptr_stk).canary_2 != canary)) {return CANARY_STK_ERROR;}
	#endif

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

	#ifdef CANARY_STK_DATA

		canary_t canary_3 = *((canary_t*) ((char*) ((*ptr_stk).data) - sizeof (canary_t)));
		canary_t canary_4 = *((canary_t*) ((char*) ((*ptr_stk).data) + (*ptr_stk).capacity * sizeof (element_t) + (8 - (*ptr_stk).capacity * sizeof (element_t) % 8) * sizeof (char)));

		if ((canary_3 != canary) || (canary_4 != canary)) {return CANARY_STK_DATA_ERROR;}
	#endif

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	#ifdef HASH_STK

		COUNT_HASH;

		if ((hash_1 != (*ptr_stk).hash_stk) || (hash_2 != (*ptr_stk).hash_stk_data)) {return HASH_ERROR;}
	#endif

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	return NOT_ERROR;
}
