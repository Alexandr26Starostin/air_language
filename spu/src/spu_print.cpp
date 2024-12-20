#include <stdio.h>
#include <assert.h>

#include "commands.h"
#include "launch_spu.h"
#include "spu_print.h"

#ifdef PRINT_SPU_
	#ifdef PRINT_CMD_
		static void print_cmd (spu_t* ptr_spu, size_t position);
	#endif

	#ifdef PRINT_REG_
		static void print_reg (spu_t* ptr_spu);
	#endif 

	#ifdef PRINT_RAM_
		static void print_ram (spu_t* ptr_spu);
	#endif

	#ifdef PRINT_STK_
		static void print_stk (spu_t* ptr_spu);
	#endif

	#ifdef PRINT_FUNC_
		static void print_func (spu_t* ptr_spu);
	#endif


//------------------------------------------------------------------------------------------------------------------------------------------


	void print_spu (spu_t* ptr_spu, size_t position)
	{
		assert (ptr_spu);

		#ifdef PRINT_CMD_
			print_cmd (ptr_spu, position);
		#endif

		#ifdef PRINT_REG_
			print_reg (ptr_spu);
		#endif
		
		#ifdef PRINT_RAM_
			print_ram (ptr_spu);
		#endif

		#ifdef PRINT_STK_
			print_stk (ptr_spu);
		#endif

		#ifdef PRINT_FUNC_
			print_func (ptr_spu);
		#endif

		#if  defined (PRINT_CMD_) || defined (PRINT_REG_) || defined (PRINT_RAM_) || defined (PRINT_STK_) || defined (PRINT_FUNC_)
			printf ("\n------------------------------------------------------------------------------------------------------------\n\n");
			getchar ();
		#endif
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------

	#ifdef PRINT_CMD_
		static void print_cmd (spu_t* ptr_spu, size_t position)
		{
			assert (ptr_spu);

			printf ("cmd:\n\n");

			for (size_t ip = 0; ip < ptr_spu -> size_cmd; ip++)
			{
				printf ("[%3ld] == %3x ", ip, (ptr_spu -> cmd)[ip]);

				if (ip == position)
				{
					printf ("<--");
				}

				printf ("\n");
			}

			printf ("\n");
		}
	#endif

	#ifdef PRINT_REG_
		static void print_reg (spu_t* ptr_spu)
		{
			assert (ptr_spu);

			printf ("reg:\n\n");

			char min_latter = 'A';
			char max_latter = 'A' - 1 + SIZE_REG;

			printf ("0X ");

			for (char latter = min_latter; latter < max_latter; latter++)
			{
				printf ("%cX ", latter);
			}

			printf ("\n");

			for (size_t index_reg = 0; index_reg < SIZE_REG; index_reg++)
			{
				printf ("%x  ", (ptr_spu -> reg)[index_reg]);
			}

			printf ("\n\n");
		}
	#endif

	#ifdef PRINT_RAM_
		static void print_ram (spu_t* ptr_spu)
		{
			assert (ptr_spu);

			printf ("ram:\n\n");

			for (size_t index_ram = 0; index_ram < SIZE_RAM; index_ram++)
			{
				printf ("[%3ld] == %3x\n", index_ram, (ptr_spu -> ram)[index_ram]);
			}
		
			printf ("\n");
		}
	#endif

	#ifdef PRINT_STK_
		static void print_stk (spu_t* ptr_spu)
		{
			assert (ptr_spu);

			printf ("stk:\n\n");

			stk_dump  (&(ptr_spu -> stk), __FILE__, __LINE__);

			printf ("\n");
		}
	#endif

	#ifdef PRINT_FUNC_
		static void print_func (spu_t* ptr_spu)
		{
			assert (ptr_spu);

			printf ("func:\n\n");

			stk_dump (&(ptr_spu -> func), __FILE__, __LINE__);
		}
	#endif
#endif