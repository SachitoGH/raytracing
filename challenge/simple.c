#include "raytracing.h"

int main(void)
{
	world w = default_world();
	// pattern p = checker_pattern(color(1, 1, 1), color(0, 0, 0));

	w.objects[2].material.reflective = 1.0f;
	// w.objects[2].material.pattern = p;

	camera cam = create_camera(1000, 500, 60);
	cam.transform = view_transform(point(0, 2, -10), point(1, 0, 0), vector(0, 1, 0));

	canvas image = low_render(cam, w, 3);
	canvas_to_ppm(&image, "scene.ppm");
	destroy_canvas(&image);
	destroy_world(&w);
	return (0);
}