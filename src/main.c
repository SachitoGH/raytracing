
#include <stdio.h>
#include <math.h>
#include "raytracing.h"  // adjust this if your header file is named differently

void test_intersect_world_with_ray(void)
{
    world w = default_world();
    ray r = create_ray(point(0, 0, -5), vector(0, 0, 1));

    intersections xs = intersect_world(w, r);

    // Check if there are 4 intersections
    if (xs.count != 4)
        printf("FAIL: Expected 4 intersections, got %d\n", xs.count);
    else
    {
        // Check t values of the intersections
        if (!equal(xs.list[0].t, 4.0))
            printf("FAIL: Expected t=4, got t=%.4f\n", xs.list[0].t);
        if (!equal(xs.list[1].t, 4.5))
            printf("FAIL: Expected t=4.5, got t=%.4f\n", xs.list[1].t);
        if (!equal(xs.list[2].t, 5.5))
            printf("FAIL: Expected t=5.5, got t=%.4f\n", xs.list[2].t);
        if (!equal(xs.list[3].t, 6.0))
            printf("FAIL: Expected t=6, got t=%.4f\n", xs.list[3].t);
    }

    printf("PASS: Intersected world with ray\n");
}



int	main(void)
{
	test_intersect_world_with_ray();
	return (0);
}