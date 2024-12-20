#ifndef HASH_H
#define HASH_H 

#include "const_define_struct.h"

//------------------------------------------------------------------

#ifdef HASH_STK
	#include "stdint.h"
	typedef uint64_t hash_t;

	#ifdef CANARY_STK_DATA
		#define COUNT_HASH                                                                                                                             \
			hash_t hash_1 = hash ((char*) ptr_stk, sizeof (stk_t) - sizeof (hash_t) * 2);                                                              \
		                                                                                                                                               \
			char* ptr_memory = (char*) (*ptr_stk).data - sizeof (canary_t);                                                                            \
			size_t len_data  = sizeof (canary_t) * 2 + (*ptr_stk).capacity * sizeof (element_t) + (8 - (*ptr_stk).capacity * sizeof (element_t) % 8) * sizeof (char);       \
		                                                                                                                                               \
			hash_t hash_2 = hash (ptr_memory, len_data);                                          
	#else   
		#define COUNT_HASH                                                                                                                             \
			hash_t hash_1 = hash ((char*) ptr_stk, sizeof (stk_t) - sizeof (hash_t) * 2);                                                              \
		                                                                                                                                               \
			char* ptr_memory = (char*) (*ptr_stk).data;                                                                                                \
			size_t len_data  = (*ptr_stk).capacity * sizeof (element_t);                                                                               \
		                                                                                                                                               \
			hash_t hash_2 = hash (ptr_memory, len_data); 
	#endif                                                                                                                                       
                                                                                                                                                       
	const hash_t number_hash = 5381;

	hash_t   hash       (char*  ptr_stk, size_t len);
	errors_t count_hash (stk_t* ptr_stk            );
#endif

//---------------------------------------------------------------------

#endif