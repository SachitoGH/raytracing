#include "raytracing.h"

bool	equal(float a, float b)
{
	if (fabsf(a - b) < EPSILON)
		return (true);
	return (false);
}

bool	compare_tuple(tuple a, tuple b)
{
	if (equal(a.x, b.x) && equal(a.y, b.y) && equal(a.z, b.z) && equal(a.w, b.w))
		return (true);
	return (false);
}

void	print_tuple(tuple t)
{
	printf("x: %.2f, y: %.2f, z: %.2f\n", t.x, t.y, t.z);
	if (t.w)
		printf("This is a point\n");
	else
		printf("This is a vector\n");
}

tuple	point(float x, float y, float z)
{
	return ((tuple){x, y, z, 1.0});
}

tuple	vector(float x, float y, float z)
{
	return ((tuple){x, y, z, 0.0});
}

tuple	sub_tuple(tuple a, tuple b)
{
	return ((tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w});
}

tuple	add_tuple(tuple a, tuple b)
{
	return ((tuple){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w});
}

tuple	div_tuple(tuple a, float d)
{
	return ((tuple){a.x / d, a.y / d, a.z / d, a.w / d});
}

tuple	mult_tuple_scalar(tuple a, float m)
{
	return ((tuple){a.x * m, a.y * m, a.z * m, a.w * m});
}

tuple	mult_tuple(tuple a, tuple b)
{
	return ((tuple){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w});
}

float	magnitude(tuple p)
{
	return (sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2) + pow(p.w, 2)));
}

tuple	normalize(tuple p)
{
	return (div_tuple(p, magnitude(p)));
}

tuple	negate_tuple(tuple a)
{
	return ((tuple){-a.x, -a.y, -a.z, -a.w});
}

float	dot(tuple a, tuple b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

tuple	cross(tuple a, tuple b)
{
	return ((tuple){a.y * b.z - a.z * b.y, 
					a.z * b.x - a.x * b.z,
					a.x * b.y - a.y * b.x,
					0.0});
}