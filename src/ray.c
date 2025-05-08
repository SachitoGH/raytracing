#include "raytracing.h"

ray	create_ray(tuple origin, tuple direction)
{
	return ((ray){origin, direction});
}

tuple	position(ray r, float t)
{
	return (add_tuple(r.origin, mult_tuple_scalar(r.direction, t)));
}

intersection	create_intersection(float t, shape s)
{
	return ((intersection){t, s});
}

intersections	create_intersections(intersection i1, intersection i2)
{
    intersections xs;
    xs.list[0] = i1;
    xs.list[1] = i2;
    xs.count = 2;
    return (xs);
}

intersection* hit(intersections* xs)
{
	intersection* hit = NULL;
	for (int i = 0; i < xs->count; ++i)
	{
		if (xs->list[i].t > EPSILON)
		{
			if (hit == NULL || xs->list[i].t < hit->t)
			{
				hit = &xs->list[i];
			}
		}
	}
	return (hit);
}

ray	transform(ray r, matrix m)
{
	r.origin = matrix_multiply_tuple(m, r.origin);
	r.direction = matrix_multiply_tuple(m, r.direction);
	return (r);
}

tuple	normal_at(shape *s, tuple world_point)
{
	return (s->normal_at(s, world_point));
}

tuple	reflect(tuple in, tuple normal)
{
	return (sub_tuple(in, mult_tuple_scalar(mult_tuple_scalar(normal, 2), dot(in, normal))));
}