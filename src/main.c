#include "raytracing.h"

int main(void)
{
	ray r = {point(2, 3, 4), vector(1, 0, 0)};

	print_tuple(position(r, 2.5));
    return (0);
}
