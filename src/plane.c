#include "raytracing.h"

shape	create_plane(void)
{
	static int	i = 0;
	shape		p;

	p.type = SHAPE_PLANE;
	p.id = i;
	p.transform = matrix_identity(4);
	p.material = create_material();
	p.intersect = &plane_intersect;
	p.normal_at = &plane_normal_at;
	return (p);
}

intersections	plane_intersect(shape *p, ray r)
{
	intersections	result;
	result.count = 0;

	if (fabsf(r.direction.y) < EPSILON)
		return(result);

	float t = -r.origin.y / r.direction.y;
	result.list[0] = create_intersection(t, *p);
	result.count++;
	return (result);
}

tuple plane_normal_at(shape *p, tuple object_point)
{
	(void)p;
	(void)object_point;
    // Local normal is always (0, 1, 0) for a plane
    return vector(0.0f, 1.0f, 0.0f);
}