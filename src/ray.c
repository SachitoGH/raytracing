#include "raytracing.h"

sphere	create_sphere(void)
{
	sphere	s;
	static int id = 0;

	s.id = id++;
	s.transform = matrix_identity(4);
	return (s);
}

ray	create_ray(tuple origin, tuple direction)
{
	return ((ray){origin, direction});
}

tuple	position(ray r, float t)
{
	return (add_tuple(r.origin, mult_tuple_scalar(r.direction, t)));
}

intersection	create_intersection(float t, sphere s)
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

intersections intersect(sphere s, ray r)
{
    intersections result;
    result.count = 0;

    ray local_ray = transform(r, inverse(s.transform));  // transform ray into object space

    tuple sphere_to_ray = sub_tuple(local_ray.origin, point(0, 0, 0));

    float a = dot(local_ray.direction, local_ray.direction);
    float b = 2 * dot(local_ray.direction, sphere_to_ray);
    float c = dot(sphere_to_ray, sphere_to_ray) - 1;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (result);

    float sqrt_disc = sqrtf(discriminant);
    float t1 = (-b - sqrt_disc) / (2 * a);
    float t2 = (-b + sqrt_disc) / (2 * a);

    result.list[0] = create_intersection(t1, s);
    result.list[1] = create_intersection(t2, s);
    result.count = 2;

    return (result);
}


intersection* hit(intersections* xs)
{
	intersection* hit = NULL;
	for (int i = 0; i < xs->count; ++i)
	{
		if (xs->list[i].t >= 0)
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

void	set_transform(sphere *s, matrix t)
{
	s->transform = t;
}