#include "raytracing.h"

canvas test(int	width, int height, int fov, int step)
{
	int	i = 0;
	world w;
	w.object_count = 1;
	w.objects = malloc(sizeof(shape) * w.object_count);
	w.light_count = 2;
	w.lights = malloc(sizeof(light)* w.light_count);
	w.lights[0] = point_light(point(3, 0, 3), color(1, 0, 0));
	w.lights[1] = point_light(point(-3, 0, 3), color(0, 1, 0));

	/*// Middle sphere
	w.objects[i] = create_sphere();
	w.objects[i].transform = translation(0, 1, 7);
	w.objects[i].material.color = color(1, 1, 1);
	w.objects[i].material.diffuse = 0.7;
	w.objects[i].material.specular = 0.3;
	i++;*/

    // Middle sphere
	w.objects[i] = create_sphere();
	w.objects[i].transform = translation(0, 0, 7);
	w.objects[i].material.color = color(1, 1, 1);
	w.objects[i].material.diffuse = 0.7;
	w.objects[i].material.specular = 0;
	i++;

	// Camera
	camera cam = create_camera(width, height, fov); // π/3 radians = 60 degrees
	cam.transform = view_transform(point(0, 0, 0), point(0, 0, 1), vector(0, 1, 0));

	// Render
	canvas	image;
	if (step > 1)
		image = low_render(cam, w, step);
	else
		image = render(cam, w);
	destroy_world(&w);
	return (image);
}
