#include "raytracing.h"  // Replace with actual headers


canvas first_scene(int width, int height, int fov, int step)
{
	int	i = 0;
	world w;
	w.object_count = 6;
	w.objects = malloc(sizeof(shape) * w.object_count);
	w.light_count = 1;
	w.lights = malloc(sizeof(light)* w.light_count);
	w.lights[0] = point_light(point(-10, 10, -10), color(1, 1, 1));
	// w.lights[1] = point_light(point(10, 10, 10), color(1, 0, 0));

	// Floor
	shape floor = create_sphere();
	floor.transform = scaling(10, 0.01, 10);
	floor.material = create_material();
	floor.material.color = color(1, 0.9, 0.9);
	floor.material.specular = 0;
	w.objects[i++] = floor;

	/*// Left wall
	shape left_wall = create_sphere();
	left_wall.transform = matrix_multiply(
		matrix_multiply(
			matrix_multiply(
				translation(0, 0, 5),
				rotation_y(-45) // in degrees
			),
			rotation_x(90)
		),
		scaling(10, 0.01, 10)
	);
	left_wall.material = floor.material;
	w.objects[i++] = left_wall;

	// Right wall
	shape right_wall = create_sphere();
	right_wall.transform = matrix_multiply(
		matrix_multiply(
			matrix_multiply(
				translation(0, 0, 5),
				rotation_y(45)
			),
			rotation_x(90)
		),
		scaling(10, 0.01, 10)
	);
	right_wall.material = floor.material;
	w.objects[i++] = right_wall;*/

	// Middle sphere
	shape middle = create_sphere();
	middle.transform = translation(-0.5, 1, 0.5);
	middle.material = create_material();
	middle.material.color = color(0.1, 1, 0.5);
	middle.material.diffuse = 0.7;
	middle.material.specular = 0.3;
	w.objects[i++] = middle;

	/*// Right sphere
	shape right = create_sphere();
	right.transform = matrix_multiply(
		translation(1.5, 0.5, -0.5),
		scaling(0.5, 0.5, 0.5)
	);
	right.material = create_material();
	right.material.color = color(0.5, 1, 0.1);
	right.material.diffuse = 0.7;
	right.material.specular = 0.3;
	w.objects[i++] = right;

	// Left sphere
	shape left = create_sphere();
	left.transform = matrix_multiply(
		translation(-1.5, 0.33, -0.75),
		scaling(0.33, 0.33, 0.33)
	);
	left.material = create_material();
	left.material.color = color(1, 0.8, 0.1);
	left.material.diffuse = 0.7;
	left.material.specular = 0.3;
	w.objects[i++] = left;*/

	// Camera
	camera cam = create_camera(width, height, fov); // π/3 radians = 60 degrees
	cam.transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

	// Render
	// canvas image = low_render(cam, w, 10);
	canvas	image;
	if (step > 1)
		image = low_render(cam, w, step);
	else
		image = render(cam, w);
	canvas_to_ppm(&image, "scene.ppm");
	destroy_world(&w);
	return (image);
}
