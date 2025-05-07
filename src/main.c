
#include <stdio.h>
#include <math.h>
#include "raytracing.h"  // adjust this if your header file is named differently


int	main(void)
{
	// Create the default world with two spheres and a light
	world w = default_world();
	w.objects[1].transform = translation(2, 0, 0);

	// Create a camera
	int width = 500;
	int height = 500;
	float fov = 60; // degrees
	camera cam = create_camera(width, height, fov);

	// Point the camera from z = -5 towards the origin, using up = +y
	tuple from = point(0, 0, -5);
	tuple to = point(2, 0, 0);
	tuple up = vector(0, 1, 0);
	cam.transform = view_transform(from, to, up);

	// Render the scene to a canvas
	canvas image = render(cam, w);

	// Write it to a PPM file
	canvas_to_ppm(&image, "scene.ppm");

	// Free the canvas memory
	destroy_canvas(&image);

	printf("PPM image saved as 'scene.ppm'\n");
	return (0);
}

