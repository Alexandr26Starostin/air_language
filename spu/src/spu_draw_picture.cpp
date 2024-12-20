#include <stdio.h>
#include <assert.h>

#include "launch_spu.h"
#include "spu_draw_picture.h"

const size_t SIZE_PICTURE = len_side_of_frame * len_side_of_frame;   // len x len

void draw_picture (spu_t* ptr_spu, size_t index_ram_for_draw)
{
	assert (ptr_spu);

	printf ("picture:\n");

	ram_t* ptr_picture = (ptr_spu -> ram) + index_ram_for_draw;

	for (size_t pixel = 0; pixel < SIZE_PICTURE; pixel++)
	{
		if (pixel % 11 == 0)
		{
			printf ("\n");
		}

		//printf ("%d ", (ptr_picture[pixel]));
		if (ptr_picture[pixel] == 0)
		{
		 	printf (". ");
		}

		else if (ptr_picture[pixel] == 1)
		{
		 	printf ("* ");
		}

		else 
		{
		 	printf (")");
		}
	}

	getchar ();
}