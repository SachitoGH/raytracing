#include "raytracing.h"

shape create_cylinder(void)
{
	static int id = 0;
	shape s;

	s.type = SHAPE_CYLINDER;
	s.id = id++;
	s.transform = matrix_identity(4);
	s.material = create_material();
	s.intersect = &cylinder_intersect;
	s.normal_at = &cylinder_normal_at;
	s.cylinder.minimum = -1;
	s.cylinder.maximum = 1;
	s.cylinder.closed = true;
	return (s);
}

bool check_cap(ray r, float t)
{
    float x = r.origin.x + t * r.direction.x;
    float z = r.origin.z + t * r.direction.z;
    return (x * x + z * z) <= 1;
}

intersections cylinder_intersect(shape *cyl, ray r) 
{
    intersections xs = { .count = 0 };
    float a = r.direction.x * r.direction.x + r.direction.z * r.direction.z;

    // Side intersections
    if (fabsf(a) > EPSILON)
	{
        float b = 2 * (r.origin.x * r.direction.x + r.origin.z * r.direction.z);
        float c = r.origin.x * r.origin.x + r.origin.z * r.origin.z - 1;
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
            if (cyl->cylinder.minimum < y0 && y0 < cyl->cylinder.maximum && xs.count < MAX_INTERSECTIONS)
                xs.list[xs.count++] = create_intersection(t0, *cyl);
            float y1 = r.origin.y + t1 * r.direction.y;
            if (cyl->cylinder.minimum < y1 && y1 < cyl->cylinder.maximum && xs.count < MAX_INTERSECTIONS)
                xs.list[xs.count++] = create_intersection(t1, *cyl);
        }
    }

    // Cap intersections (if closed)
    if (cyl->cylinder.closed && fabsf(r.direction.y) > EPSILON) 
	{
        float t_lower = (cyl->cylinder.minimum - r.origin.y) / r.direction.y;
        if (check_cap(r, t_lower) && xs.count < MAX_INTERSECTIONS)
            xs.list[xs.count++] = create_intersection(t_lower, *cyl);
        float t_upper = (cyl->cylinder.maximum - r.origin.y) / r.direction.y;
        if (check_cap(r, t_upper) && xs.count < MAX_INTERSECTIONS)
            xs.list[xs.count++] = create_intersection(t_upper, *cyl);
    }

    return xs;
}

tuple cylinder_normal_at(shape *s, tuple object_point)
{
	float dist = object_point.x * object_point.x + object_point.z * object_point.z;

	// Check if it's the side (caps not handled yet)
	if (dist < 1 && object_point.y >= s->cylinder.maximum - EPSILON)
		return vector(0, 1, 0); // Top cap (to be implemented)

	if (dist < 1 && object_point.y <= s->cylinder.minimum + EPSILON)
		return vector(0, -1, 0); // Bottom cap (to be implemented)

	return vector(object_point.x, 0, object_point.z);
}

