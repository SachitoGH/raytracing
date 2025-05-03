#ifndef raytracing
#define raytracing

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 0.00001f


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
bool	equal_tuple(tuple a, tuple b);
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


// matrix.c
#define MAX_MATRIX 4

typedef struct
{
	int		size;
	float	data[MAX_MATRIX][MAX_MATRIX];
}	matrix;

void print_matrix(matrix m);
tuple matrix_multiply_tuple(matrix m, tuple t);
matrix matrix_multiply(matrix a, matrix b);
int matrix_equal(matrix a, matrix b);
matrix matrix_zero(int size);
matrix matrix_identity(int size);
matrix matrix_transpose(matrix m);
matrix inverse(matrix m);
float minor(matrix m, int row, int col);
int is_invertible(matrix m);
float cofactor(matrix m, int row, int col);
matrix submatrix(matrix m, int row, int col);
float determinant_2x2(matrix m);
float determinant(matrix m);


//transformation
matrix translation(float tx, float ty, float tz);
matrix scaling(float sx, float sy, float sz);
matrix rotation_x(float angle);
matrix rotation_y(float angle);
matrix rotation_z(float angle);

//raytracing.h
typedef struct
{
	tuple	origin;
	tuple	direction;
}	ray;

ray	create_ray(tuple origin, tuple direction);
tuple	position(ray r, float t);

#endif