#include "raytracing.h"

pattern	stripe_pattern(tuple a, tuple b)
{
	pattern p;
	p.a = a;
	p.b = b;
	p.transform = matrix_identity(4);
	return (p);
}

tuple	stripe_at(pattern s, tuple p)
{
	if (fmodf(floorf(p.x), 2.0f) == 0.0f)
		return (s.a);
	else
		return (s.b);
}

tuple	stripe_at_object(pattern p, shape s, tuple world_point)
{
	tuple	object_point = matrix_multiply_tuple(inverse(s.transform), world_point);
	tuple 	p_point = matrix_multiply_tuple(inverse(p.transform), object_point);

	return (stripe_at(p, p_point));
}
