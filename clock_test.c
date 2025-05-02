#include "raytracing.h"

tuple	conversion(float rad, tuple p)
{
	p.x *= rad;
	p.z *= rad;
	p.x += 25;
	p.z += 25;
	return (p);
}

int	main(void)
{
	canvas canvas = create_canvas(50, 50);
	tuple	red = color(1, 0, 0);
	matrix	rotation = rotation_y(30);
	tuple	hour_point = point(0,0,1);
	tuple	canvas_point;

	for (int i = 0; i < 12; i++)
	{
		canvas_point = conversion(20, hour_point);
		write_pixel(&canvas, canvas_point.x, canvas_point.z, red);
		hour_point = matrix_multiply_tuple(rotation, hour_point);
	}
	canvas_to_ppm(&canvas, "test.ppm");
	return (0);

}