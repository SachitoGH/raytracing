#include "raytracing.h"


int main(void)
{
	pattern test = ring_pattern(color(1, 1, 1), color(0,0,0));

	print_tuple(test.pattern_at(&test, point(0.7,0,0.7)));
	return(0);
}