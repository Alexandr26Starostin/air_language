#ifndef OPERATIONS_WITH_STACK_H
#define OPERATIONS_WITH_STACK_H

#include "const_define_struct.h"

//----------------------------------------------------------------------------------------------------------------------------------------------

#define PRINT_INF_ABOUT_STK                                                         \
		printf ("\t ptr_stk  == %p             \n", ptr_stk            );            \
		printf ("\t ptr_data == %p             \n", (*ptr_stk).data    );            \
		printf ("\t size     == %ld            \n", (*ptr_stk).size    );            \
		printf ("\t capacity == %ld            \n", (*ptr_stk).capacity);            \
		printf ("\t len type of element == %ld\n",  sizeof (element_t));  
	
#ifdef CANARY_STK
	#define PRINT_CANARY_STK                                                   \
		printf ("\t canary_1 == %lx\n", (*ptr_stk).canary_1);                    \
		printf ("\t canary_2 == %lx\n", (*ptr_stk).canary_2);                    
#else
	#define PRINT_CANARY_STK 
#endif

#ifdef CANARY_STK_DATA
	#define PRINT_CANARY_STK_DATA                                                                                                                                     \
		canary_t canary_3 = *((canary_t*) ((char*) (*ptr_stk).data - sizeof (canary_t)));                                                                              \
		canary_t canary_4 = *((canary_t*) ((char*) ((*ptr_stk).data) + (*ptr_stk).capacity * sizeof (element_t) + (8 - (*ptr_stk).capacity * sizeof (element_t) % 8) * sizeof (char)));     \
                                                                                                                                                                       \
		printf ("\t canary_3 == %lx\n", canary_3);                                                                                                                      \
		printf ("\t canary_4 == %lx\n", canary_4);                                                                                                                      
#else
	#define PRINT_CANARY_STK_DATA
#endif

#ifdef HASH_STK
	#define PRINT_HASH                                                      \
		printf ("\t hash_stk      == %ld\n",   (*ptr_stk).hash_stk);        \
		printf ("\t hash_stk_data == %ld\n\n", (*ptr_stk).hash_stk_data);   
	#else 
		#define PRINT_HASH
#endif 

//----------------------------------------------------------------------------------------------------------------------------------

#define PRINT                 \
	PRINT_INF_ABOUT_STK       \
	PRINT_CANARY_STK          \
	PRINT_CANARY_STK_DATA     \
	PRINT_HASH                \
	printf ("\n");

//----------------------------------------------------------------------------------------------------------------------------------

const int number_up   = 2;
const int number_down = 4;

errors_t stk_ctor (stk_t* ptr_stk, size_t         start_capacity               );
errors_t stk_dtor (stk_t* ptr_stk                                              );
errors_t stk_dump (stk_t* ptr_stk, const char*    file,          const int line);
errors_t stk_pop  (stk_t* ptr_stk, element_t*     ptr_element                  );
errors_t stk_push (stk_t* ptr_stk, element_t      element                      );

#endif
