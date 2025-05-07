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
canvas	upscale_canvas(canvas *src, int scale_x, int scale_y);


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
	tuple	position;
	tuple	intensity;
}	light;

typedef struct
{
	tuple	color;
	float	ambient;
	float	diffuse;
	float	specular;
	float	shininess;
}	material;

typedef struct
{
	tuple	origin;
	tuple	direction;
}	ray;

typedef	struct
{
	int			id;
	matrix		transform;
	material	material;
}	sphere;


typedef struct 
{
	float	t;
	sphere	object;
}	intersection;

#define MAX_INTERSECTIONS 100

typedef struct 
{
	intersection	list[MAX_INTERSECTIONS];
	int				count;
}	intersections;


ray				create_ray(tuple origin, tuple direction);
tuple			position(ray r, float t);
sphere			create_sphere(void);
intersections	intersect(sphere s, ray r);
intersection	create_intersection(float t, sphere object);
intersections	make_intersections(intersection i1, intersection i2);
intersection*	hit(intersections* xs);
ray				transform(ray r, matrix m);
void	set_transform(sphere *s, matrix t);
tuple	normal_at(sphere s, tuple p);
tuple	reflect(tuple in, tuple normal);


light		point_light(tuple position, tuple intensity);
material	create_material(void);
tuple	lighting(material m, light l, tuple p, tuple eyev, tuple normalv, bool in_shadow);

// scene.c

typedef struct
{	
	sphere	*objects;
	light	*lights;
	int		object_count;
	int		light_count;
}	world;

typedef struct s_computation
{
	float	t;
	sphere	object;     // the intersected object
	tuple	point;      // the point of intersection
	tuple	eyev;       // the eye (view) vector
	tuple	normalv;    // the normal vector at the point
	tuple	over_point;
	bool	inside;     // true if the intersection occurs inside the object
}	computation;

world default_world(void);
void	destroy_world(world *w);
intersections intersect_world(world w, ray r);
computation	prepare_computations(intersection i, ray r);
tuple		shade_hit(world w, computation c);
tuple	color_at(world w, ray r);
matrix view_transform(tuple from, tuple to, tuple up);

typedef	struct
{
	int hsize;
	int vsize;
	float fov;
	matrix transform;
	float pixel_size;
	float half_width;
	float half_height;
}	camera;

camera	create_camera(int hsize, int vsize, float fov);
ray ray_for_pixel(camera cam, int px, int py);
canvas render(camera cam, world w);

//shadow.c
bool is_shadowed(world w, tuple p, light l);


#endif