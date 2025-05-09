#include "raytracing.h"

int main(void)
{
	world w = default_world();

	w.objects[1].material.pattern = stripe_pattern(color(1, 1, 1), color(0, 0, 0));
	w.objects[1].material.pattern_flag = true;
	w.objects[1].transform = matrix_multiply(w.objects[1].transform, rotation_z(45));
	camera cam = create_camera(1000, 500, 60);
	cam.transform = view_transform(point(0, 2, -10), point(1, 0, 0), vector(0, 1, 0));


	canvas image = low_render(cam, w, 4);
	canvas_to_ppm(&image, "scene.ppm");
	destroy_canvas(&image);
	destroy_world(&w);
	return (0);
}