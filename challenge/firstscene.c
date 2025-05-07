#include "raytracing.h"  // Replace with actual headers

int main(void)
{
	world w;
	w.object_count = 0;
	w.light_source = point_light(point(-10, 10, -10), color(1, 1, 1));

	// Floor
	sphere floor = create_sphere();
	floor.transform = scaling(10, 0.01, 10);
	floor.material = create_material();
	floor.material.color = color(1, 0.9, 0.9);
	floor.material.specular = 0;
	w.objects[w.object_count++] = floor;

	// Left wall
	sphere left_wall = create_sphere();
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
	w.objects[w.object_count++] = left_wall;

	// Right wall
	sphere right_wall = create_sphere();
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
	w.objects[w.object_count++] = right_wall;

	// Middle sphere
	sphere middle = create_sphere();
	middle.transform = translation(-0.5, 1, 0.5);
	middle.material = create_material();
	middle.material.color = color(0.1, 1, 0.5);
	middle.material.diffuse = 0.7;
	middle.material.specular = 0.3;
	w.objects[w.object_count++] = middle;

	// Right sphere
	sphere right = create_sphere();
	right.transform = matrix_multiply(
		translation(1.5, 0.5, -0.5),
		scaling(0.5, 0.5, 0.5)
	);
	right.material = create_material();
	right.material.color = color(0.5, 1, 0.1);
	right.material.diffuse = 0.7;
	right.material.specular = 0.3;
	w.objects[w.object_count++] = right;

	// Left sphere
	sphere left = create_sphere();
	left.transform = matrix_multiply(
		translation(-1.5, 0.33, -0.75),
		scaling(0.33, 0.33, 0.33)
	);
	left.material = create_material();
	left.material.color = color(1, 0.8, 0.1);
	left.material.diffuse = 0.7;
	left.material.specular = 0.3;
	w.objects[w.object_count++] = left;

	// Camera
	camera cam = create_camera(1000, 500, 60); // π/3 radians = 60 degrees
	cam.transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

	// Render
	canvas image = render(cam, w);
	canvas_to_ppm(&image, "scene.ppm");
	destroy_canvas(&image);
	return (0);
}
