#include "raytracing.h"

int	main(void)
{
	tuple	color1 = color(1, 0.2, 0.4);
	tuple	color2 = color(0.9, 1, 0.1);
	tuple	test = mult_tuple(color1, color2);

	print_tuple(test);
	return (0);
}