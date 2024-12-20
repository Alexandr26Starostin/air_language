#ifndef CONST_DEFINE_STRUCT_H
#define CONST_DEFINE_STRUCT_H

/*
#define DEBUG_ASSERT
#define CANARY_STK_NOT
#define CANARY_STK_DATA_NOT
#define HASH_STK_NOT
#define MEMSET_NOT
*/

//------------------------------------------------------------------

#ifdef HASH_STK
	#include "stdint.h"

	typedef uint64_t hash_t;
#endif

//--------------------------------------------------------------------

#ifdef CANARY_STK 

	#include "stdint.h"

	typedef uint64_t canary_t;

	const canary_t canary = 0xC0FFEE;

#endif
//----------------------------------------------------------------------

typedef long element_t;

struct stk_t 
{
	#ifdef CANARY_STK
		canary_t canary_1;
	#endif 

	size_t     size;
	size_t     capacity;
	element_t* data;

	#ifdef CANARY_STK
		canary_t canary_2;
	#endif 

	#ifdef HASH_STK
		hash_t hash_stk;
		hash_t hash_stk_data;
	#endif
};

//-----------------------------------------------------------------------

#ifdef CANARY_STK_DATA 
    
	#ifndef CANARY_STK
		#include "stdint.h"
		typedef uint64_t canary_t;

		const canary_t canary = 0xC0FFEE0FF1CE;
	#endif

#endif

//-----------------------------------------------------------------------

enum errors_t 
{
	NOT_ERROR                 = 0,
	FALL_PTR_DATA             = 1,
	SIZE_MORE_CAPACITY        = 2,

	CTOR_ERROR                = 11,    
	DUMP_WRITE_ERROR          = 12,
	DTOR_ERROR                = 13,
	REALLOC_ERROR_BACK        = 14,
	REALLOC_ERROR_FORWARD     = 15,
	PUSH_ERROR                = 16,
	POP_ERROR                 = 17,

	#ifdef CANARY_STK
		CANARY_STK_ERROR      = 101,
	#endif

	#ifdef CANARY_STK_DATA
		CANARY_STK_DATA_ERROR = 102,
	#endif

	#ifdef HASH_STK
		HASH_ERROR            = 1001,
	#endif

	PTR_MEMORY_ERROR          = 10001,
	MEMSET_ERROR              = 10002
};

//------------------------------------------------------------------------

#include "operations_with_stack.h"
#include "stk_error.h"

#ifdef DEBUG_ALL
	#define STK_ASSERT(ptr_stk, __FILE__, __LINE__)

#else
	#ifdef DEBUG_ASSERT
		#define STK_ASSERT(ptr_stk, __FILE__, __LINE__)        \
																  \
			if ((stk_error (ptr_stk)) != NOT_ERROR)               \
			{                                                     \
				stk_dump (ptr_stk, __FILE__, __LINE__);           \
				return (stk_error (ptr_stk));                     \
			}                                
	#else 
		#define STK_ASSERT(ptr_stk, __FILE__, __LINE__)         \
			if ((stk_error (ptr_stk)) != NOT_ERROR)               \
			{                                                     \
				stk_dump (ptr_stk, __FILE__, __LINE__);           \
				assert (NULL);                                    \
			}   
	#endif
#endif

//--------------------------------------------------------------------------

const int poison = 0xBAD; // BIG

//--------------------------------------------------------------------------

#endif
