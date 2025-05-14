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
	return ((intersection){s, t});
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
	for (int i = 0; i < xs->count; ++i)
	{
		if (xs->list[i].t > EPSILON)
		{
			return	(&xs->list[i]);
		}
	}
	return (NULL);
}

ray	transform(ray r, matrix *m)
{
	r.origin = matrix_multiply_tuple(m, r.origin);
	r.direction = matrix_multiply_tuple(m, r.direction);
	return (r);
}

tuple normal_at(shape *s, tuple world_point)
{
    // Transform the world point to object coordinates
    matrix inv = inverse(&s->transform);
    tuple object_point = matrix_multiply_tuple(&inv, world_point);

    // Get the local normal from the shape's specific normal_at function
    tuple object_normal = s->normal_at(s, object_point);

    // Transform the normal to world coordinates using inverse transpose
	matrix transpose = matrix_transpose(&inv);
    tuple world_normal = matrix_multiply_tuple(&transpose, object_normal);

    // Ensure it's a vector
    world_normal.w = 0;

    // Normalize the world normal
    return normalize(world_normal);
}

tuple	reflect(tuple in, tuple normal)
{
	return (sub_tuple(in, mult_tuple_scalar(mult_tuple_scalar(normal, 2), dot(in, normal))));
}