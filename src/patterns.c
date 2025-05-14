#include "raytracing.h"

tuple	stripe_at(pattern *self, tuple point)
{
	if ((int)floorf(point.x) % 2 == 0)
		return (self->a);
	else
		return (self->b);
}

pattern	stripe_pattern(tuple a, tuple b)
{
	pattern p;
	p.type = PATTERN_STRIPE;
	p.a = a;
	p.b = b;
	p.transform = matrix_identity(4);
	p.pattern_at = stripe_at;
	return (p);
}


pattern	gradient_pattern(tuple a, tuple b)
{
	pattern p;
	p.type = PATTERN_GRADIENT;
	p.a = a;
	p.b = b;
	p.transform = matrix_identity(4);
	p.pattern_at = gradient_at;
	return (p);
}

tuple	gradient_at(pattern *p, tuple point)
{
	tuple distance = sub_tuple(p->b, p->a);
	float fraction = point.x - floorf(point.x);
	return (add_tuple(p->a, mult_tuple_scalar(distance, fraction)));
}

pattern	ring_pattern(tuple a, tuple b)
{
	pattern p;
	p.type = PATTERN_RING;
	p.a = a;
	p.b = b;
	p.transform = matrix_identity(4);
	p.pattern_at = ring_at;
	return (p);
}

tuple ring_at(pattern *p, tuple point)
{
    float dist = sqrtf(point.x * point.x + point.z * point.z);
    if (((int)floorf(dist + EPSILON)) % 2 == 0)
        return (p->a);
    else
        return (p->b);
}

pattern	checker_pattern(tuple a, tuple b)
{
	pattern p;
	p.type = PATTERN_CHECKER;
	p.a = a;
	p.b = b;
	p.transform = matrix_identity(4);
	p.pattern_at = checker_at;
	return (p);
}

tuple	checker_at(pattern *p, tuple point)
{
	int x = (int)floorf(point.x + EPSILON);
	int y = (int)floorf(point.y + EPSILON);
	int z = (int)floorf(point.z + EPSILON);
	int sum = x + y + z;

	if ((sum % 2) == 0)
		return (p->a);
	else
		return (p->b);
}



tuple	pattern_at_object(pattern *p, shape s, tuple world_point)
{
	matrix	temp;

	temp = inverse(&s.transform);
	tuple object_point = matrix_multiply_tuple(&temp, world_point);
	temp = inverse(&p->transform);
	tuple pattern_point = matrix_multiply_tuple(&temp, object_point);
	return (p->pattern_at(p, pattern_point));
}