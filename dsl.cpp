DEF_CMD_(push, 0x01, true,
	{
		int arg = get_arg_push (ptr_spu, &ip);
		stk_push (&(ptr_spu -> stk), (element_t) arg);
	})

DEF_CMD_(pop, 0x0C, true,
	{
		long arg = 0;
		stk_pop (&(ptr_spu -> stk), &arg);
		*get_arg_pop (ptr_spu, &ip) = (int) arg;
	})

DEF_CMD_(guide, 0x00, false,
	{
		char str[MAX_LETTERS] = "";
		while (fgets (str, MAX_LETTERS, guide_file) != NULL) {printf ("%s", str);}
		printf ("\n");
	})

//---------------------------------------------------------------------------------------------------

#define SIMPLE_CMD_WITH_TWO_ARG_(name_cmd, operation)                                   \
	{                                    \
		element_t a = 0, b = 0;         \
		stk_pop (&(ptr_spu -> stk), &a);                   \
		stk_pop (&(ptr_spu -> stk), &b);                   \
                                                              \
		stk_push (&(ptr_spu -> stk), b operation a);       \
	}


DEF_CMD_(add, 0x02, false, SIMPLE_CMD_WITH_TWO_ARG_(ADD, +))

DEF_CMD_(sub, 0x03, false, SIMPLE_CMD_WITH_TWO_ARG_(SUB, -))

DEF_CMD_(mul, 0x04, false, SIMPLE_CMD_WITH_TWO_ARG_(MUL, *))

DEF_CMD_(div, 0x05, false, SIMPLE_CMD_WITH_TWO_ARG_(DIV, /))

#undef SIMPLE_CMD_WITH_TWO_ARG_

//--------------------------------------------------------------------------------------------------------

DEF_CMD_(idiv, 0x14, false,
	{
		element_t a = 0, b = 0;         
		stk_pop (&(ptr_spu -> stk), &a);         
		stk_pop (&(ptr_spu -> stk), &b);

		stk_push (&(ptr_spu -> stk), (int) (b / a));
	})

DEF_CMD_(out, 0x06, false,
	{
		element_t arg = 0;      
		stk_pop (&(ptr_spu -> stk), &arg);

		printf ("%ld\n", arg);
		getchar ();
	})

DEF_CMD_(in, 0x07, false,
	{
		element_t arg = 0;
		scanf ("%ld", &arg);
		stk_push (&(ptr_spu -> stk), arg);
	})

DEF_CMD_(mau, 0x19, false,
	{
		element_t arg = 0;      
		stk_pop (&(ptr_spu -> stk), &arg);

		for (long index_mau = 0; index_mau < arg; index_mau++)
		{
			printf ("mau\n");
		}
	})

DEF_CMD_(sqrt, 0x08, false,
	{
		element_t arg = 0;      
		stk_pop (&(ptr_spu -> stk), &arg);

		stk_push (&(ptr_spu -> stk), (long) sqrt ((double) arg));
	})

DEF_CMD_(sin, 0x09, false,
	{
		element_t arg = 0;      
		stk_pop (&(ptr_spu -> stk), &arg);

		stk_push (&(ptr_spu -> stk), (long) sin ((double) arg));
	})

DEF_CMD_(cos, 0x0A, false,
	{
		element_t arg = 0;      
		stk_pop (&(ptr_spu -> stk), &arg);

		stk_push (&(ptr_spu -> stk), (long) cos ((double) arg));
	})

DEF_CMD_(hlt, 0x0B, false,
	{
		fclose (guide_file);
		return ptr_spu -> error_in_spu;
	})

DEF_CMD_(jmp, 0x0D, true,
	{
		ip = (ptr_spu -> cmd)[ip + 1] - 1;
	})

DEF_CMD_(call, 0x10, true,
	{
		stk_push (&(ptr_spu -> func), (long) (ip + 1));
		ip = (ptr_spu -> cmd)[ip + 1] - 1;
	})

DEF_CMD_(ret, 0x11, false,
	{
		long ret_value = 0;
		stk_pop (&(ptr_spu -> func), &ret_value);
		ip = ret_value;
	})

DEF_CMD_(draw, 0x12, true,
	{
		size_t index_ram_for_draw = (size_t) (ptr_spu -> cmd)[ip + 1];
		ip += 1;
		draw_picture (ptr_spu, index_ram_for_draw);
	})

DEF_CMD_(dump, 0x13, false,
	{
		stk_dump (&(ptr_spu -> stk), __FILE__, __LINE__);
	})

//--------------------------------------------------------------------------------------------------------

#define JUMP_CMD_(operation)                             \
	{                                                 \
		element_t a = 0, b = 0;          \
		stk_pop (&(ptr_spu -> stk), &a);        \
		stk_pop (&(ptr_spu -> stk), &b);                 \
                                                         \
		if (a operation b)                                    \
		{                                                  \
			ip = (ptr_spu -> cmd)[ip + 1] - 1;                \
		}                                                      \
		else                                                    \
		{                                                         \
			ip += 1;                                            \
		}                                                           \
	}

DEF_CMD_(ja, 0x0E, true, JUMP_CMD_(>))                   //Нижний большего верхнего

DEF_CMD_(jb, 0x0F, true, JUMP_CMD_(<))                   //Верхний больше нижнего

DEF_CMD_(jae, 0x15, true, JUMP_CMD_(>=))         //Нижний не менее верхнего

DEF_CMD_(jbe, 0x16, true, JUMP_CMD_(<=))          //Верхний не меньше нижнего

DEF_CMD_(je, 0x17, true, JUMP_CMD_(==))            //Верхний равен нижнему

DEF_CMD_(jne, 0x18, true, JUMP_CMD_(!=))           //Верхний не равен нижнему


#undef JUMP_CMD_