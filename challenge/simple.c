#include "raytracing.h"

canvas simple(int width, int height, int fov, int step)
{
	world w = default_world();

	// w.objects[1].material = create_glass_material();
	camera cam = create_camera(width, height, fov);
	set_camera_transform(&cam, view_transform(point(0, 2, -10), point(0, 0, 0), vector(0, 1, 0)));

	canvas	image;
	(void)step;

	image = thread_render(cam, w);
	// if (step > 1)
	// 	image = low_render(cam, w, step);
	// else
	// 	image = render(cam, w);
	destroy_world(&w);
	return (image);
}
