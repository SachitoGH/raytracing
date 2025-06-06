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


static bool refracted_ray(computation* comps, ray* out_ray) {
    float n_ratio = comps->n1 / comps->n2;
    float cos_i = dot(comps->eyev, comps->normalv);
    float sin2_t = n_ratio * n_ratio * (1.0f - cos_i * cos_i);

    // Total internal reflection if sin2_t > 1
    if (sin2_t > 1.0f) {
        return false;
    }

    float cos_t = sqrtf(1.0f - sin2_t);
    tuple direction = add_tuple(
        mult_tuple_scalar(comps->normalv, n_ratio * cos_i - cos_t),
        mult_tuple_scalar(comps->eyev, -n_ratio)
    );
    *out_ray = create_ray(comps->under_point, normalize(direction));
    return true;
}

// Schlick approximation for Fresnel effect
static float schlick(computation* comps)
{
    float cos_i = dot(comps->eyev, comps->normalv);

    // Adjust cos_i for total internal reflection
    if (comps->n1 > comps->n2)
	{
        float n_ratio = comps->n1 / comps->n2;
        float sin2_t = n_ratio * n_ratio * (1.0f - cos_i * cos_i);
        if (sin2_t > 1.0f)
            return 1.0f; // Total reflection
        float cos_t = sqrtf(1.0f - sin2_t);
        cos_i = cos_t; // Use cos_t for grazing angles
    }

    float r0 = powf((comps->n1 - comps->n2) / (comps->n1 + comps->n2), 2);
    float term = powf(1.0f - cos_i, 5);
    return r0 + (1.0f - r0) * term;
}

tuple refracted_color(world w, computation comps, int remaining)
{
    if (comps.object.material.transparency < EPSILON || remaining <= 0) {
        return color(0.0f, 0.0f, 0.0f);
    }

    ray refract_ray;
    if (!refracted_ray(&comps, &refract_ray)) {
        return color(0.0f, 0.0f, 0.0f); // Total internal reflection
    }

    tuple refract_color = color_at(w, refract_ray, remaining - 1);
    return mult_tuple_scalar(refract_color, comps.object.material.transparency);
}

tuple shade_hit(world w, computation c, int remaining)
{
    tuple surface_color = color(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < w.light_count; i++)
	{
        surface_color = add_tuple(
            surface_color,
            lighting(
                c.object.material,
                c.object,
                w.lights[i],
                c.point,
                c.eyev,
                c.normalv,
                is_shadowed(w, c.over_point, w.lights[i])
            )
        );
    }

    // Compute reflected and refracted colors
    tuple reflected = reflected_color(w, c, remaining);
    tuple refracted = refracted_color(w, c, remaining);

    // Combine colors using Schlick approximation for transparent materials
    if (c.object.material.transparency > EPSILON)
	{
        float reflectance = schlick(&c);
        tuple reflect_component = mult_tuple_scalar(reflected, reflectance);
        tuple refract_component = mult_tuple_scalar(refracted, 1.0f - reflectance);
        return add_tuple(surface_color, add_tuple(reflect_component, refract_component));
    }

    // For opaque materials, combine surface and reflected colors
    return add_tuple(surface_color, reflected);
}

tuple	color_at(world w, ray r, int remaining)
{
	intersections	xs = intersect_world(w, r);
	intersection	*i = hit(&xs);
	if (!i)
		return (color(0.0f, 0.0f, 0.0f));
	computation		c;
	c.t = i->t;
	c.object = i->object;
	prepare_computations(&c, r, xs);
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

static void init_containers(containers* c)
{
    c->count = 0;
}

static void add_container(containers* c, shape* obj)
{
    if (c->count < MAX_INTERSECTIONS)
        c->objects[c->count++] = obj;
}

static void remove_container(containers* c, shape* obj)
{
    for (int i = 0; i < c->count; i++)
	{
        if (c->objects[i] == obj)
		{
            c->objects[i] = c->objects[--c->count]; // Replace with last
            break;
        }
    }
}

static bool contains(containers* c, shape* obj)
{
    for (int i = 0; i < c->count; i++)
        if (c->objects[i] == obj)
			return true;
    return false;
}

static void compute_refractive_indices(computation* comps, intersections xs)
{
	containers containers;

    init_containers(&containers);

    for (int i = 0; i < xs.count; i++) {
        intersection* current = &xs.list[i];

        // Check if current intersection is the hit
        if (equal(current->t, comps->t) && current->object.id == comps->object.id) {
            // Set n1: refractive index of medium ray is coming from
            if (containers.count == 0) {
                comps->n1 = 1.0f; // Air
            } else {
                comps->n1 = containers.objects[containers.count - 1]->material.refractive_index;
            }

            // Set n2: refractive index of medium ray is entering
            if (contains(&containers, &comps->object)) {
                remove_container(&containers, &comps->object);
                if (containers.count == 0) {
                    comps->n2 = 1.0f; // Exiting into air
                } else {
                    comps->n2 = containers.objects[containers.count - 1]->material.refractive_index;
                }
            } else {
                comps->n2 = comps->object.material.refractive_index; // Entering hit object
            }
            break; // Stop after processing the hit
        }

        // Toggle container
        if (contains(&containers, &current->object)) {
            remove_container(&containers, &current->object);
        } else {
            add_container(&containers, &current->object);
        }
    }
}

void prepare_computations(computation *comps, ray r, intersections xs)
{
    comps->point = position(r, comps->t);
    comps->eyev = negate_tuple(r.direction);
    comps->normalv = normal_at(&comps->object, comps->point);
    comps->inside = dot(comps->normalv, comps->eyev) < EPSILON;
    if (comps->inside)
        comps->normalv = negate_tuple(comps->normalv);

    // Avoid shadow acne by pushing the point slightly above the surface
    tuple offset = mult_tuple_scalar(comps->normalv, 0.01f);
    comps->over_point = add_tuple(comps->point, offset);
    comps->under_point = sub_tuple(comps->point, offset);
    comps->reflectv = reflect(r.direction, comps->normalv);

    // Compute n1 and n2 for refraction
	compute_refractive_indices(comps, xs);
}

tuple ray_for_pixel(camera cam, matrix *inv, tuple origin, int px, int py)
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
	timer t;
	start_timer(&t);
	canvas	image = create_canvas(cam.hsize, cam.vsize);
	ray		r;

	r.origin = cam.origin;
	for (int y = 0; y < cam.vsize; y++)
	{
		for (int x = 0; x < cam.hsize; x++)
		{
			r.direction = ray_for_pixel(cam, &cam.inverse_transform, r.origin, x, y);
			write_pixel(&image, x, y, color_at(w, r, 4));
		}
	}
	print_elapsed_time(&t);
	return image;
}

canvas low_render(camera cam, world w, int step)
{
	timer t;
	start_timer(&t);
	canvas	image = create_canvas(cam.hsize, cam.vsize);
	ray		r;

	r.origin = cam.origin;
    // Render every 'step'th pixel
    for (int y = 0; y < cam.vsize; y += step)
    {
        for (int x = 0; x < cam.hsize; x += step)
        {
            r.direction = ray_for_pixel(cam, &cam.inverse_transform, r.origin, x, y);
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
	print_elapsed_time(&t);
    return image;
}