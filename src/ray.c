#include "raytracing.h"

sphere	create_sphere(void)
{
	sphere	s;
	static int id = 0;

	s.id = id++;
	s.transform = matrix_identity(4);
	s.material = create_material();
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
    if (discriminant < EPSILON)
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
		if (xs->list[i].t > EPSILON)
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

tuple	normal_at(sphere s, tuple world_point)
{
	matrix	inv = inverse(s.transform);
	tuple	object_point = matrix_multiply_tuple(inv, world_point);
	tuple	object_normal = sub_tuple(object_point, point(0, 0, 0));
	tuple	world_normal = matrix_multiply_tuple(matrix_transpose(inv), object_normal);
	world_normal.w = 0;
	return (normalize(world_normal));
}

tuple	reflect(tuple in, tuple normal)
{
	return (sub_tuple(in, mult_tuple_scalar(mult_tuple_scalar(normal, 2), dot(in, normal))));
}

light	point_light(tuple position, tuple intensity)
{
	return ((light){position, intensity});
}

material	create_material(void)
{
	material	m;
	m.color = color(1, 1, 1);
	m.ambient = 0.1;
	m.diffuse = 0.9;
	m.specular = 0.9;
	m.shininess = 200.0;
	return (m);
}

tuple	lighting(material m, light l, tuple p, tuple eyev, tuple normalv, bool in_shadow)
{
	tuple	diffuse;
	tuple	specular;
	tuple	effective_color = mult_tuple(m.color, l.intensity);
	tuple	lightv = normalize(sub_tuple(l.position, p));
	tuple	ambient = mult_tuple_scalar(effective_color, m.ambient);
	float	light_dot_normal = dot(lightv, normalv);

	if (light_dot_normal < EPSILON || in_shadow)
		return (ambient);
	diffuse = mult_tuple_scalar(effective_color, m.diffuse * light_dot_normal);
	tuple	reflectv = reflect(negate_tuple(lightv), normalv);
	float	reflect_dot_eye = dot(reflectv, eyev);
	if (reflect_dot_eye <= EPSILON)
		specular = color(0, 0, 0);
	else
	{
		float factor = pow(reflect_dot_eye, m.shininess);
		specular = mult_tuple_scalar(mult_tuple_scalar(l.intensity, m.specular), factor);
	}
	return (add_tuple(add_tuple(ambient, diffuse), specular));
}