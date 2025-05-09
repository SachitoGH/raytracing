#include "raytracing.h"
#include <time.h>

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
    if (first != NULL && first->t < distance)
    {
        return true; // Point is in shadow for this light
    }
    return false; // No shadow
}


tuple		shade_hit(world w, computation c)
{
	tuple	res = color(0, 0, 0);
	for (int i = 0; i < w.light_count; i++)
		res = add_tuple(res, lighting(c.object.material, w.lights[i], c.point, c.eyev, c.normalv, is_shadowed(w, c.over_point, w.lights[i])));
	return (res);
}

tuple	color_at(world w, ray r)
{
	intersections	xs = intersect_world(w, r);
	intersection	*i = hit(&xs);
	if (!i)
		return (color(0, 0, 0));
	computation		c = prepare_computations(*i, r);
	return (shade_hit(w, c));
}

computation	prepare_computations(intersection i, ray r)
{
	computation comps;

	comps.t = i.t;
	comps.object = i.object;
	comps.point = position(r, comps.t);
	comps.eyev = negate_tuple(r.direction);
	comps.normalv = normal_at(&comps.object, comps.point);

	if (dot(comps.normalv, comps.eyev) < EPSILON)
	{
		comps.inside = true;
		comps.normalv = negate_tuple(comps.normalv);
	}
	else
	{
		comps.inside = false;
	}

	// Avoid shadow acne by pushing the point slightly above the surface
	tuple offset = mult_tuple_scalar(comps.normalv, 0.01f);
	comps.over_point = add_tuple(comps.point, offset);

	return (comps);
}

tuple dir_for_pixel(camera *cam, matrix *inv, tuple *origin, int px, int py)
{
    // Offset from the edge of the canvas to the pixel center
    float xoffset = (px + 0.5f) * cam->pixel_size;
    float yoffset = (py + 0.5f) * cam->pixel_size;

    // Untransformed coordinates of the pixel in world space
    float world_x = cam->half_width - xoffset;
    float world_y = cam->half_height - yoffset;

    // Using the camera matrix, transform the canvas point and the origin
    tuple pixel = matrix_multiply_tuple(*inv, point(world_x, world_y, -1));
    return normalize(sub_tuple(pixel, *origin));
}

canvas render(camera cam, world w)
{
	canvas	image = create_canvas(cam.hsize, cam.vsize);
	matrix	inv = inverse(cam.transform);
	clock_t	start = clock();
	ray		r;

	printf("Rendering...\n");
	r.origin = matrix_multiply_tuple(inv, point(0, 0, 0));
	for (int y = 0; y < cam.vsize; y++)
	{
		for (int x = 0; x < cam.hsize; x++)
		{
			r.direction = dir_for_pixel(&cam, &inv, &r.origin,x, y);
			write_pixel(&image, x, y, color_at(w, r));
		}
	}
	printf("Time: %.6f\n", (clock() - start) / (float) CLOCKS_PER_SEC);
	return image;
}
/*
canvas low_render(camera cam, world w, int step)
{
    canvas	image = create_canvas(cam.hsize, cam.vsize);
	matrix	inv = inverse(cam.transform);
	clock_t	start = clock();
	tuple	color_at_pixel;
	ray		r;

	printf("Rendering...\n");
	r.origin = matrix_multiply_tuple(inv, point(0, 0, 0));
    // Render every 'step'th pixel
    for (int y = 0; y < cam.vsize; y += step)
    {
        for (int x = 0; x < cam.hsize; x += step)
        {
            r.direction = dir_for_pixel(&cam, &inv, &r.origin,x, y);
            color_at_pixel = color_at(w, r);

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
}*/