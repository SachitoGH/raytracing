#include "raytracing.h"  // Replace with actual headers

int main(void)
{
	int	i = 0;
	world w = default_world();

	// Camera
	camera cam = create_camera(100, 50, 60); // π/3 radians = 60 degrees
	cam.transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

	// Render
	canvas image = render(cam, w);
	canvas highres = upscale_canvas(&image, 10, 10);
	canvas_to_ppm(&highres, "scene.ppm");
	destroy_canvas(&image);
	destroy_world(&w);
	return (0);
}
