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

intersections sphere_intersect(shape *s, ray local_ray)
{
    intersections result;
    result.count = 0;

    tuple sphere_to_ray = sub_tuple(local_ray.origin, point(0.0f, 0.0f, 0.0f));

    float a = dot(local_ray.direction, local_ray.direction);
    float b = dot(local_ray.direction, sphere_to_ray);
    float c = dot(sphere_to_ray, sphere_to_ray) - 1;

    float discriminant = b * b -  a * c;
    if (discriminant < EPSILON)
        return (result);

    float sqrt_disc = sqrtf(discriminant);
    float t1 = (-b - sqrt_disc) / (a);
    float t2 = (-b + sqrt_disc) / (a);

    result.list[0] = create_intersection(t1, *s);
    result.list[1] = create_intersection(t2, *s);
    result.count = 2;

    return (result);
}

tuple sphere_normal_at(shape *s, tuple object_point)
{
	(void)s;
    // Local normal for a sphere is the vector from center to point
	object_point.w -= 1.0f;
    return object_point;
}