#include "raytracing.h"

world default_world(void)
{
    world w;
    
    // Create a light source
    w.light_count = 1;
	w.lights = malloc(sizeof(light) * w.light_count);
	w.lights[0] = point_light(point(-10, 10, -10), color(1, 1, 1));
    
    // Allocate memory for 2 spheres
    w.object_count = 2;
    w.objects = malloc(sizeof(shape) * w.object_count);
    
    // First sphere: default material and transformation
    w.objects[0] = create_sphere(); // Using the create_sphere function from your previous code
    w.objects[0].material.color = color(0.8, 1.0, 0.6); // Example color
    w.objects[0].material.diffuse = 0.7;
	w.objects[0].material.specular = 0.2;
    
    // Second sphere: scaled, with a different material
    w.objects[1] = create_sphere();
    set_transform(&w.objects[1], scaling(0.5, 0.5, 0.5)); // Scale the second sphere
    
    return w;
}

void	destroy_world(world *w)
{
	free(w->objects);
	w->objects = NULL;
}

light	point_light(tuple position, tuple intensity)
{
	return ((light){position, intensity});
}

void sort_intersections(intersections *xs)
{
    for (int i = 0; i < xs->count - 1; i++)
    {
        for (int j = 0; j < xs->count - i - 1; j++)
        {
            if (xs->list[j].t > xs->list[j + 1].t)
            {
                // Swap the intersections
                intersection temp = xs->list[j];
                xs->list[j] = xs->list[j + 1];
                xs->list[j + 1] = temp;
            }
        }
    }
}

intersections intersect_world(world w, ray r)
{
    intersections xs;
    xs.count = 0;

    // Intersect the ray with each object in the world
    for (int i = 0; i < w.object_count; i++)
    {
        intersections temp = intersect(&w.objects[i], r);
        for (int j = 0; j < temp.count; j++)
        {
            xs.list[xs.count++] = temp.list[j];
        }
    }

    // Sort the intersections by their 't' value
    sort_intersections(&xs);

    return xs;
}

intersections	intersect(shape *object, ray r)
{
	return (object->intersect(object, r));
}

matrix view_transform(tuple from, tuple to, tuple up)
{
	tuple forward = normalize(sub_tuple(to, from));
	tuple upn = normalize(up);
	tuple left = cross(forward, upn);
	tuple true_up = cross(left, forward);

	matrix orientation = matrix_identity(4);
	orientation.data[0][0] = left.x;
	orientation.data[0][1] = left.y;
	orientation.data[0][2] = left.z;

	orientation.data[1][0] = true_up.x;
	orientation.data[1][1] = true_up.y;
	orientation.data[1][2] = true_up.z;

	orientation.data[2][0] = -forward.x;
	orientation.data[2][1] = -forward.y;
	orientation.data[2][2] = -forward.z;

	matrix translation_matrix = translation(-from.x, -from.y, -from.z);
	return (matrix_multiply(orientation, translation_matrix));
}


camera	create_camera(int hsize, int vsize, float fov)
{
	camera cam;
	cam.hsize = hsize;
	cam.vsize = vsize;
	cam.fov = fov * (M_PI / 180.0f); //convert to rad
	cam.transform = matrix_identity(4);

	float half_view = tanf(cam.fov / 2);
	float aspect = (float)hsize / (float)vsize;

	if (aspect >= 1)
	{
		cam.half_width = half_view;
		cam.half_height = half_view / aspect;
	}
	else
	{
		cam.half_width = half_view * aspect;
		cam.half_height = half_view;
	}
	cam.pixel_size = (cam.half_width * 2) / hsize;

	return (cam);
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
