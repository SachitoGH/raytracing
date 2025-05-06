#include "raytracing.h"

int main(void)
{
    ray r = create_ray(point(0, 0, -5), vector(0, 0, 1));
    int wall_z = 10;
    int canvas_pixels = 100;
    float wall_size = 7;
    float pixel_size = wall_size / canvas_pixels;
    float half = wall_size / 2;

    canvas c = create_canvas(canvas_pixels, canvas_pixels);
    tuple red = color(1, 0, 0);
    sphere s = create_sphere();
	s.transform = scaling(1, 0.5, 1);

    for (int y = 0; y < canvas_pixels; y++)  // Corrected to go through all pixels
    {
        float world_y = half - pixel_size * y;
        for (int x = 0; x < canvas_pixels; x++)  // Corrected to go through all pixels
        {
            float world_x = -half + pixel_size * x;
            tuple position = point(world_x, world_y, wall_z);  // This is needed to calculate the direction
            r = create_ray(r.origin, normalize(sub_tuple(position, r.origin)));  // Use existing ray, corrected
            intersections xs = intersect(s, r);
            if (hit(&xs))  // Corrected to pass pointer to intersections
                write_pixel(&c, x, y, red);
        }
    }
    
    canvas_to_ppm(&c, "chapter5.ppm");  // Corrected to pass by value
}