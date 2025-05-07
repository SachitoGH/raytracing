
#include <stdio.h>
#include <math.h>
#include "raytracing.h"  // adjust this if your header file is named differently


int main(void)
{
    world w = default_world(); // Assume this world has multiple lights

	// Scenario 1: No shadow when an object is behind the light
	tuple p1 = point(-20, 20, -20);  // The point behind the light
	if (is_shadowed(w, p1, w.lights[0]))
	{
		printf("The point is in shadow.\n");
	}
	else
	{
		printf("The point is not in shadow.\n");
	}

	// Scenario 2: No shadow when an object is behind the point
	tuple p2 = point(-2, 2, -2);  // The point with an object behind it
	if (is_shadowed(w, p2, w.lights[0]))
	{
		printf("The point is in shadow.\n");
	}
	else
	{
		printf("The point is not in shadow.\n");
	}

    return 0;
}
