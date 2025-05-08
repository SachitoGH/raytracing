#include "raytracing.h"

shape	create_sphere(void)
{
	shape	s;
	static int id = 0;

	s.type = SHAPE_SPHERE;
	s.id = id++;
	s.transform = matrix_identity(4);
	s.material = create_material();
	s.intersect = &sphere_intersect;
	s.normal_at = &sphere_normal_at;
	return (s);
}

intersections sphere_intersect(shape *s, ray r)
{
    intersections result;
    result.count = 0;

    ray local_ray = transform(r, inverse(s->transform));  // transform ray into object space

    tuple sphere_to_ray = sub_tuple(local_ray.origin, point(0, 0, 0));

    float a = dot(local_ray.direction, local_ray.direction);
    float b = 2 * dot(local_ray.direction, sphere_to_ray);
    float c = dot(sphere_to_ray, sphere_to_ray) - 1;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < EPSILON)
        return (result);

    float sqrt_disc = sqrtf(discriminant);
    float t1 = (-b - sqrt_disc) / (2 * a);
    float t2 = (-b + sqrt_disc) / (2 * a);

    result.list[0] = create_intersection(t1, *s);
    result.list[1] = create_intersection(t2, *s);
    result.count = 2;

    return (result);
}

tuple	sphere_normal_at(shape *s, tuple world_point)
{
	matrix	inv = inverse(s->transform);
	tuple	object_point = matrix_multiply_tuple(inv, world_point);
	tuple	object_normal = sub_tuple(object_point, point(0, 0, 0));
	tuple	world_normal = matrix_multiply_tuple(matrix_transpose(inv), object_normal);
	world_normal.w = 0;
	return (normalize(world_normal));
}