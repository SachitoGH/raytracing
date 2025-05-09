#include "raytracing.h"

int main(void)
{
	world w = default_world();
	pattern p = ring_pattern(color(1, 1, 1), color(0, 0, 0));
	p.transform = scaling(0.2, 0.2, 0.2);

	w.objects[0].material.pattern = p;
	w.objects[0].transform = matrix_multiply(translation(3,0,0), scaling(1.5,1.5,1.5));

	w.objects[1].material.pattern = p;
	w.objects[2].material.pattern = p;
	// w.objects[1].transform = matrix_multiply(w.objects[1].transform, rotation_z(45));
	camera cam = create_camera(1000, 500, 60);
	cam.transform = view_transform(point(0, 2, -10), point(1, 0, 0), vector(0, 1, 0));


	canvas image = low_render(cam, w, 1);
	canvas_to_ppm(&image, "scene.ppm");
	destroy_canvas(&image);
	destroy_world(&w);
	return (0);
}