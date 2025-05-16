#include "raytracing.h"

canvas color_scene(int width, int height, int fov) // WORK IN PROGRESS
{
    world	w;

    // Create a light source
    w.light_count = 3;
	w.lights = malloc(sizeof(light) * w.light_count);
	w.lights[0] = point_light(point(-1.0f, 0.0f, -20.0f), color(1.0f, 0.0f, 0.0f));
	w.lights[1] = point_light(point(-1.0f, 0.0f, -20.0f), color(0.0f, 1.0f, 0.0f));
	w.lights[2] = point_light(point(-1.0f, 0.0f, -20.0f), color(0.0f, 0.0f, 1.0f));
    
    // Allocate memory for object
    w.object_count = 2;
    w.objects = malloc(sizeof(shape) * w.object_count);

	w.objects[0] = create_sphere();
	
	w.objects[1] = create_plane();
	set_transform(&w.objects[1], rotation_x(90));

	camera cam = create_camera(width, height, fov);
	cam.transform = view_transform(point(0, 2, -10), point(0, 0, 0), vector(0, 1, 0));

	canvas image = thread_render(cam, w);
	return (image);
}