#include "raytracing.h"

tuple	lighting(material m, shape object, light l, tuple p, tuple eyev, tuple normalv, bool in_shadow)
{
	tuple	diffuse;
	tuple	specular;
	tuple	col;

	if (m.pattern.type != UNSET)
		col = pattern_at_object(&m.pattern, object, p);
	else
		col = m.color;
	tuple	effective_color = mult_tuple(col, l.intensity);
	tuple	ambient = mult_tuple_scalar(effective_color, m.ambient);
	if (in_shadow)
		return (ambient);
	tuple	lightv = normalize(sub_tuple(l.position, p));
	float	light_dot_normal = dot(lightv, normalv);
	if (light_dot_normal < EPSILON)
		return (ambient);
	diffuse = mult_tuple_scalar(effective_color, m.diffuse * light_dot_normal);
	tuple	reflectv = reflect(negate_tuple(lightv), normalv);
	float	reflect_dot_eye = dot(reflectv, eyev);
	if (reflect_dot_eye <= EPSILON)
		specular = color(0.0f, 0.0f, 0.0f);
	else
	{
		float factor = pow(reflect_dot_eye, m.shininess);
		specular = mult_tuple_scalar(mult_tuple_scalar(l.intensity, m.specular), factor);
	}
	return (add_tuple(add_tuple(ambient, diffuse), specular));
}

bool is_shadowed(world w, tuple p, light l)
{
    tuple direction = sub_tuple(l.position, p);  // Vector from point to light
    float distance = magnitude(direction);  // Distance from the point to the light
    direction = normalize(direction);  // Normalize the direction vector

    // Create a ray from the point to the light
    ray r = create_ray(p, direction);

    // Check for intersections with objects in the world
    intersections xs = intersect_world(w, r);

    // If there's an intersection and it's closer than the light source, the point is in shadow
    intersection* first = hit(&xs);
    return (first != NULL && first->t < distance); // True: In shadow
}


tuple		shade_hit(world w, computation c, int remaining)
{
	tuple	res = color(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < w.light_count; i++)
	{
		res = add_tuple(res, lighting(c.object.material, c.object, w.lights[i], c.point, c.eyev, c.normalv, is_shadowed(w, c.over_point, w.lights[i])));
		res = add_tuple(res, reflected_color(w, c, remaining));
	}
	return (res);
}

tuple	color_at(world w, ray r, int remaining)
{
	intersections	xs = intersect_world(w, r);
	intersection	*i = hit(&xs);
	if (!i)
		return (color(0.0f, 0.0f, 0.0f));
	computation		c = prepare_computations(*i, r);
	return (shade_hit(w, c, remaining));
}

tuple reflected_color(world w, computation comps, int remaining)
{
    if (comps.object.material.reflective < EPSILON || remaining <= 0)
        return (color(0, 0, 0));

    ray reflect_ray = create_ray(comps.over_point, comps.reflectv);
    tuple color = color_at(w, reflect_ray, remaining - 1);
    return (mult_tuple_scalar(color, comps.object.material.reflective));
}


computation	prepare_computations(intersection i, ray r)
{
	computation comps;

	comps.t = i.t;
	comps.object = i.object;
	comps.point = position(r, comps.t);
	comps.eyev = negate_tuple(r.direction);
	comps.normalv = normal_at(&comps.object, comps.point);
	comps.inside = dot(comps.normalv, comps.eyev) < EPSILON;
	if (comps.inside)
		comps.normalv = negate_tuple(comps.normalv);

	// Avoid shadow acne by pushing the point slightly above the surface
	tuple offset = mult_tuple_scalar(comps.normalv, 0.01f);
	comps.over_point = add_tuple(comps.point, offset);
	comps.reflectv = reflect(r.direction, comps.normalv);

	return (comps);
}

tuple ray_for_pixel(camera cam, matrix inv, tuple origin, int px, int py)
{
    // Offset from the edge of the canvas to the pixel center
    float xoffset = (px + 0.5f) * cam.pixel_size;
    float yoffset = (py + 0.5f) * cam.pixel_size;

    // Untransformed coordinates of the pixel in world space
    float world_x = cam.half_width - xoffset;
    float world_y = cam.half_height - yoffset;

    // Using the camera matrix, transform the canvas point and the origin
    tuple pixel = matrix_multiply_tuple(inv, point(world_x, world_y, -1.0f));
    tuple direction = normalize(sub_tuple(pixel, origin));

    return direction;
}

canvas render(camera cam, world w)
{
	canvas	image = create_canvas(cam.hsize, cam.vsize);
	matrix	inv = inverse(cam.transform);
	ray		r;

	r.origin = matrix_multiply_tuple(inv, point(0.0f, 0.0f, 0.0f));
	for (int y = 0; y < cam.vsize; y++)
	{
		for (int x = 0; x < cam.hsize; x++)
		{
			r.direction = ray_for_pixel(cam, inv, r.origin, x, y);
			write_pixel(&image, x, y, color_at(w, r, 4));
		}
	}
	return image;
}

canvas low_render(camera cam, world w, int step)
{
    canvas image = create_canvas(cam.hsize, cam.vsize);
	matrix inv = inverse(cam.transform);
	ray		r;

	r.origin = matrix_multiply_tuple(inv, point(0.0f, 0.0f, 0.0f));
    // Render every 'step'th pixel
    for (int y = 0; y < cam.vsize; y += step)
    {
        for (int x = 0; x < cam.hsize; x += step)
        {
            r.direction = ray_for_pixel(cam, inv, r.origin, x, y);
            tuple color_at_pixel = color_at(w, r, 4);

            // Write the computed color to a block of pixels
            for (int dy = 0; dy < step && y + dy < cam.vsize; dy++)
            {
                for (int dx = 0; dx < step && x + dx < cam.hsize; dx++)
                {
                    write_pixel(&image, x + dx, y + dy, color_at_pixel);
                }
            }
        }
    }

    return image;
}