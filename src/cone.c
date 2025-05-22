#include "raytracing.h"

#include "raytracing.h"

shape create_cone(void)
{
	static int id = 0;
	shape s;

	s.type = SHAPE_CONE;
	s.id = id++;
	set_transform(&s, matrix_identity(4));
	s.material = create_material();
	s.intersect = &cone_intersect;
	s.normal_at = &cone_normal_at;
	s.cylinder.minimum = -1;
	s.cylinder.maximum = 1;
	s.cylinder.closed = false;
	return (s);
}

bool check_cone_cap(ray r, float t, float radius)
{
	float x = r.origin.x + t * r.direction.x;
	float z = r.origin.z + t * r.direction.z;
	return (x * x + z * z) <= radius * radius;
}

intersections cone_intersect(shape *cone, ray r)
{
	intersections xs = { .count = 0 };
	float a = r.direction.x * r.direction.x - r.direction.y * r.direction.y + r.direction.z * r.direction.z;
	float b = 2 * (r.origin.x * r.direction.x - r.origin.y * r.direction.y + r.origin.z * r.direction.z);
	float c = r.origin.x * r.origin.x - r.origin.y * r.origin.y + r.origin.z * r.origin.z;

	if (fabsf(a) > EPSILON)
	{
		float disc = b * b - 4 * a * c;
		if (disc >= 0)
		{
			float t0 = (-b - sqrtf(disc)) / (2 * a);
			float t1 = (-b + sqrtf(disc)) / (2 * a);
			if (t0 > t1)
			{
				float tmp = t0;
				t0 = t1;
				t1 = tmp;
			}
			float y0 = r.origin.y + t0 * r.direction.y;
			if (cone->cylinder.minimum < y0 && y0 < cone->cylinder.maximum && xs.count < MAX_INTERSECTIONS)
				xs.list[xs.count++] = create_intersection(t0, *cone);
			float y1 = r.origin.y + t1 * r.direction.y;
			if (cone->cylinder.minimum < y1 && y1 < cone->cylinder.maximum && xs.count < MAX_INTERSECTIONS)
				xs.list[xs.count++] = create_intersection(t1, *cone);
		}
	}
	else if (fabsf(b) > EPSILON)
	{
		float t = -c / (2 * b);
		float y = r.origin.y + t * r.direction.y;
		if (cone->cylinder.minimum < y && y < cone->cylinder.maximum && xs.count < MAX_INTERSECTIONS)
			xs.list[xs.count++] = create_intersection(t, *cone);
	}

	// Cap intersections
	if (cone->cylinder.closed && fabsf(r.direction.y) > EPSILON)
	{
		float t_lower = (cone->cylinder.minimum - r.origin.y) / r.direction.y;
		if (check_cone_cap(r, t_lower, fabsf(cone->cylinder.minimum)) && xs.count < MAX_INTERSECTIONS)
			xs.list[xs.count++] = create_intersection(t_lower, *cone);

		float t_upper = (cone->cylinder.maximum - r.origin.y) / r.direction.y;
		if (check_cone_cap(r, t_upper, fabsf(cone->cylinder.maximum)) && xs.count < MAX_INTERSECTIONS)
			xs.list[xs.count++] = create_intersection(t_upper, *cone);
	}

	return (xs);
}

tuple cone_normal_at(shape *s, tuple p)
{
	float dist = p.x * p.x + p.z * p.z;

	if (dist < 1 && p.y >= s->cylinder.maximum - EPSILON)
		return vector(0, 1, 0); // Top cap
	if (dist < 1 && p.y <= s->cylinder.minimum + EPSILON)
		return vector(0, -1, 0); // Bottom cap

	float y = sqrtf(p.x * p.x + p.z * p.z);
	if (p.y > 0)
		y = -y;

	return vector(p.x, y, p.z);
}
