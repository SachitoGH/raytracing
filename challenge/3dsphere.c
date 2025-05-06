#include "raytracing.h"

int main(void)
{
	// setup the scene
	sphere s = create_sphere();
	material m = create_material();
	m.color = color(1, 0.2, 1);
	s.material = m;
	//s.transform = scaling(1, 0.5, 1);

	light l = point_light(point(-10, 10, -10), color(1, 1, 1));

	//camera and canvas
	int canvas_pixels = 1000;
	float wall_z = 10.0;
	float wall_size = 7.0;
	float pixel_size = wall_size / canvas_pixels;
	float half = wall_size / 2;

	canvas c = create_canvas(canvas_pixels, canvas_pixels);
	tuple ray_origin = point(0, 0, -5);

	//loop through each pixel
	for (int y = 0; y < canvas_pixels; y++)
	{
		float world_y = half - pixel_size * y;
		for (int x = 0; x < canvas_pixels; x++)
		{
			float world_x = -half + pixel_size * x;
			tuple pos = point(world_x, world_y, wall_z);
			tuple direction = normalize(sub_tuple(pos, ray_origin));
			ray r = create_ray(ray_origin, direction);

			intersections xs = intersect(s, r);
			intersection *h = hit(&xs);

			if (h)
			{
				tuple p = position(r, h->t);
				tuple normal = normal_at(h->object, p);
				tuple eye = negate_tuple(r.direction);
				tuple color_at_hit = lighting(h->object.material, l, p, eye, normal);
				write_pixel(&c, x, y, color_at_hit);
			}
		}
	}

	canvas_to_ppm(&c, "chapter6.ppm");
	destroy_canvas(&c);
}