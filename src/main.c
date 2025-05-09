#include "raytracing.h"


int main(void)
{
	material m;
	shape	sphere = create_sphere();

	m.pattern = stripe_pattern(color(1, 1, 1), color(0, 0, 0));
	m.pattern_flag = true;
	m.ambient = 1;
	m.diffuse = 0;
	m.specular = 0;
	tuple eyev = vector(0, 0, -1);
	tuple normalv = vector(0, 0, -1);
	sphere.material = m;
	light l = point_light(point(0, 0, -10), color(1, 1, 1));
	tuple c1 = lighting(m, sphere, l, point(0.9, 0, 0), eyev, normalv, false);
	tuple c2 = lighting(m, sphere, l, point(1.1, 0, 0), eyev, normalv, false);
	print_tuple(c1);
	print_tuple(c2);
	return(0);
}