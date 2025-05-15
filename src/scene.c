#include "raytracing.h"

world default_world(void)
{
    world	w;
	matrix	temp1;
	matrix	temp2;
    
    // Create a light source
    w.light_count = 1;
	w.lights = malloc(sizeof(light) * w.light_count);
	w.lights[0] = point_light(point(-10.0f, 10.0f, -10.0f), color(1.0f, 1.0f, 1.0f));
    
    // Allocate memory for 1 sphere, 1 cube, 1 cylinder and a 3 planes
    w.object_count = 5;
    w.objects = malloc(sizeof(shape) * w.object_count);
    
    //  sphere
    w.objects[0] = create_sphere(); // Using the create_sphere function from your previous code
	temp1 = scaling(1, 1, 1);
	temp2 = translation(-2.0f, 0.0f, 0.0f);
	w.objects[0].transform = matrix_multiply(&temp1, &temp2);
    
    // cube
    w.objects[1] = create_cube();
	w.objects[1].material.color = color(1.0f, 0.6f, 0.0f);
	temp1 = translation(3.0f, 0.0f, -1.0f);
	temp2 = rotation_y(45.0f);
	w.objects[1].transform = matrix_multiply(&temp1, &temp2);
	// w.objects[1].material.pattern = checker_pattern(color(1, 1, 1), color(0, 0, 0));
    // w.objects[1].transform = scaling(0.5f, 0.5f, 0.5f);

	// cylinder
	w.objects[2] = create_cylinder();
	w.objects[2].material.color = color(0.6f, 1.0f, 0.0f);
	w.objects[2].transform = translation(-3.5f, 0.0f, -3.0f);

	//plane floor
	w.objects[3] = create_plane();
	w.objects[3].material.color = color(1.0f, 1.0f, 1.0f);
	w.objects[3].material.reflective = 0.3f;
	w.objects[3].material.pattern = checker_pattern(color(1, 1, 1), color(0, 0, 0));;
	temp1 = translation(0.0f, -1.0f, 0.0f);
	temp2 = rotation_x(0.0f);
	w.objects[3].transform = matrix_multiply(&temp1, &temp2);

	//plane front wall
	w.objects[4] = create_plane();
	w.objects[4].material.color = color(1.0f, 1.0f, 1.0f);
	w.objects[4].material.pattern = gradient_pattern(color(1, 0, 0), color(0, 0, 1));
	w.objects[4].material.pattern.transform = scaling(20, 1, 1);
	temp1 = translation(10.0f, 0.0f, 3.0f);
	temp2 = rotation_x(90.0f);
	w.objects[4].transform = matrix_multiply(&temp1, &temp2);
	
    return w;
}

void	destroy_world(world *w)
{
	if (w->objects)
	{
		free(w->objects);
		w->objects = NULL;
	}
	if (w->lights)
	{
		free(w->lights);
		w->lights = NULL;
	}
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
	intersections temp;
    xs.count = 0;

    // Intersect the ray with each object in the world
    for (int i = 0; i < w.object_count; i++)
    {
        temp = intersect(&w.objects[i], r);
        for (int j = 0; j < temp.count; j++)
            xs.list[xs.count++] = temp.list[j];
    }

    // Sort the intersections by their 't' value
    sort_intersections(&xs);

    return xs;
}

intersections intersect(shape *object, ray r)
{
	matrix	temp = inverse(&object->transform);
	ray	local_ray = transform(r, &temp);
    return (object->intersect(object, local_ray));
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
	return (matrix_multiply(&orientation, &translation_matrix));
}


camera	create_camera(int hsize, int vsize, float fov)
{
	camera cam;
	cam.hsize = hsize;
	cam.vsize = vsize;
	cam.fov = fov * DEG_RADIANTS; //convert to rad
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
	m.ambient = 0.1f;
	m.diffuse = 0.9f;
	m.specular = 0.9f;
	m.shininess = 200.0f;
	m.pattern.type = UNSET;
	m.reflective = 0.0f;
	return (m);
}
