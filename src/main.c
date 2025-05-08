
#include <stdio.h>
#include <math.h>
#include "raytracing.h"  // adjust this if your header file is named differently

void	test_hit_offsets_point(void)
{
	ray r = create_ray(point(0, 0, -5), vector(0, 0, 1));
	shape s = create_sphere();
	set_transform(&s, translation(0, 0, 1));
	intersection i = create_intersection(5, s);
	computation comps = prepare_computations(i, r);

	if (comps.over_point.z < -EPSILON / 2 && comps.point.z > comps.over_point.z)
		printf("PASS: hit offset point (over_point.z < -EPSILON/2 and point.z > over_point.z)\n");
	else
	{
		printf("FAIL: over_point.z = %f, point.z = %f\n", comps.over_point.z, comps.point.z);
	}
}


int main(void)
{
    test_hit_offsets_point();

    return 0;
}
