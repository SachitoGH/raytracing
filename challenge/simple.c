#include "raytracing.h"

canvas simple(int width, int height, int fov, int step)
{
	world w = default_world();

	// w.objects[1].material.pattern = checker_pattern(color);
	camera cam = create_camera(width, height, fov);
	cam.transform = view_transform(point(0, 2, -10), point(0, 0, 0), vector(0, 1, 0));

	canvas	image;
	(void)step;

	image = thread_render(cam, w);
	// if (step > 1)
	// 	image = low_render(cam, w, step);
	// else
	// 	image = render(cam, w);
	// canvas_to_ppm(&image, "scene.ppm");
	destroy_world(&w);
	return (image);
}
