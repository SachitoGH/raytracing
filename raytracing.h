#ifndef raytracing
#define raytracing

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 0.00001


// tuples.c

typedef	struct
{
	float	x;
	float	y;
	float	z;
	float	w;
}	tuple;

tuple	point(float x, float y, float z);
tuple	vector(float x, float y, float z);

tuple	sub_tuple(tuple a, tuple b);
tuple	add_tuple(tuple a, tuple b);
tuple	div_tuple(tuple a, float d);
tuple	mult_tuple_scalar(tuple a, float m);
tuple	mult_tuple(tuple a, tuple b);
tuple	negate_tuple(tuple a);

float	magnitude(tuple p);
tuple	normalize(tuple p);
float	dot(tuple a, tuple b);
tuple	cross(tuple a, tuple b);

bool	equal(float a, float b);
bool	compare_tuple(tuple a, tuple b);
void	print_tuple(tuple t);

// drawing.c

typedef	struct
{
	int		width;
	int		height;
	tuple	*pixels;
}	canvas;

canvas	create_canvas(int w, int h);
void	destroy_canvas(canvas* c);
tuple	pixel_at(canvas* c, int x, int y);
void	write_pixel(canvas* c, int x, int y, tuple col);
tuple	color(float r, float g, float b);
void	canvas_to_ppm(canvas* c, const char* filename);
int		to_ppm_component(float value);



#endif