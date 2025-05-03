#include "raytracing.h"

ray	create_ray(tuple origin, tuple direction)
{
	return ((ray){origin, direction});
}

tuple	position(ray r, float t)
{
	return (add_tuple(r.origin, mult_tuple_scalar(r.direction, t)));
}