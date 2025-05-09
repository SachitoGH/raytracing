#include "raytracing.h"

tuple	stripe_at(pattern *s, tuple p)
{
	if (fmodf(floorf(p.x), 2.0f) == 0.0f)
		return (s->a);
	else
		return (s->b);
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

tuple	pattern_at_object(pattern *p, shape s, tuple world_point)
{
	tuple	object_point = matrix_multiply_tuple(inverse(s.transform), world_point);
	tuple	pattern_point = matrix_multiply_tuple(inverse(p->transform), object_point);
	return (p->pattern_at(p, pattern_point));
}