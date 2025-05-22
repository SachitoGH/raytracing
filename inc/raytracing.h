#ifndef raytracing
# define raytracing

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include "../minilibx/mlx.h"
# include <time.h>

# define EPSILON 0.00001f
# define ESC_KEY 65307
# define DEG_RADIANTS 0.017453293

// struct

typedef	struct
{
	float	x;
	float	y;
	float	z;
	float	w;
}	tuple;


typedef	struct
{
	int		width;
	int		height;
	tuple	*pixels;
}	canvas;

#define MAX_MATRIX 4

typedef struct
{
	int		size;
	float	data[MAX_MATRIX][MAX_MATRIX];
}	matrix;


typedef enum {
	UNSET,
	PATTERN_STRIPE,
	PATTERN_GRADIENT,
	PATTERN_RING,
	PATTERN_CHECKER
	// Add more as needed
} pattern_type;

typedef struct pattern pattern;

struct pattern
{
	matrix			transform;
	tuple			a;  // Color A
	tuple			b;  // Color B
	tuple			(*pattern_at)(pattern *self, tuple point);
	pattern_type	type;
};

typedef struct
{
	tuple	position;
	tuple	intensity;
}	light;

typedef struct
{
	tuple	color;
	pattern	pattern;
	float	ambient;
	float	diffuse;
	float	specular;
	float	shininess;
	float	reflective;
	float	transparency;
	float	refractive_index;
}	material;

typedef struct
{
	tuple	origin;
	tuple	direction;
}	ray;

typedef enum {
	SHAPE_SPHERE,
	SHAPE_PLANE,
	SHAPE_CUBE,
	SHAPE_CYLINDER,
	SHAPE_CONE
	//..
} shape_type;


typedef struct
{
	float minimum;
	float maximum;
	bool closed;
} cylinder_props;

// Forward declaration of shape and intersections
typedef struct s_shape shape;
typedef struct s_intersections intersections;

typedef struct s_shape
{
    shape_type type;
    matrix transform;
	matrix inverse_transform;
    material material;

    // Use function pointers with forward-declared intersection type
    intersections (*intersect)(shape *self, ray r);
    tuple (*normal_at)(shape *self, tuple world_point);
	int id;
	union
	{
		cylinder_props cylinder;
		//maybe other later
	};
} shape;

typedef struct s_intersection
{
    shape object;
	float t;
} intersection;

#define MAX_INTERSECTIONS 50

typedef struct s_intersections
{
	intersection	list[MAX_INTERSECTIONS];
	int				count;
}	intersections;

typedef struct
{
    shape* objects[MAX_INTERSECTIONS];
    int count;
} containers;

typedef struct
{	
	shape	*objects;
	light	*lights;
	int		object_count;
	int		light_count;
}	world;

typedef struct s_computation
{
	shape	object;     // the intersected object
	tuple	point;      // the point of intersection
	tuple	eyev;       // the eye (view) vector
	tuple	normalv;    // the normal vector at the point
	tuple	over_point;
	tuple	under_point;
	tuple	reflectv;
	float	t;
	bool	inside;     // true if the intersection occurs inside the object
	float	n1;
	float	n2;
}	computation;

typedef	struct
{
	matrix transform;
	matrix inverse_transform;
	tuple origin;
	int hsize;
	int vsize;
	float fov;
	float pixel_size;
	float half_width;
	float half_height;
}	camera;


// tuples.c

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

canvas	create_canvas(int w, int h);
void	destroy_canvas(canvas* c);
tuple	pixel_at(canvas* c, int x, int y);
void	write_pixel(canvas* c, int x, int y, tuple col);
tuple	color(float r, float g, float b);
void	canvas_to_ppm(canvas* c, const char* filename);
int		to_ppm_component(float value);
canvas	upscale_canvas(canvas *src, int scale_x, int scale_y);


// matrix.c

void print_matrix(matrix *m);
tuple matrix_multiply_tuple(matrix *m, tuple t);
matrix matrix_multiply(matrix *a, matrix *b);
int matrix_equal(matrix *a, matrix *b);
matrix matrix_zero(int size);
matrix matrix_identity(int size);
matrix matrix_transpose(matrix *m);
matrix inverse(matrix *m);
float minor(matrix *m, int row, int col);
int is_invertible(matrix *m);
float cofactor(matrix *m, int row, int col);
matrix submatrix(matrix *m, int row, int col);
float determinant_2x2(matrix *m);
float determinant(matrix *m);


//transformation.c
void	set_transform(shape *s, matrix t);
matrix translation(float tx, float ty, float tz);
matrix scaling(float sx, float sy, float sz);
matrix rotation_x(float angle);
matrix rotation_y(float angle);
matrix rotation_z(float angle);

//scene.h

world default_world(void);
void	destroy_world(world *w);
light	point_light(tuple position, tuple intensity);
void sort_intersections(intersections *xs);
intersections intersect_world(world w, ray r);
intersections	intersect(shape *object, ray r);
matrix view_transform(tuple from, tuple to, tuple up);
camera	create_camera(int hsize, int vsize, float fov);
void set_camera_transform(camera *cam, matrix transform);

// ray.c
ray	create_ray(tuple origin, tuple direction);
tuple	position(ray r, float t);
intersection	create_intersection(float t, shape s);
intersections	create_intersections(intersection i1, intersection i2);
intersection* hit(intersections* xs);
ray	transform(ray r, matrix *m);
tuple	normal_at(shape *s, tuple world_point);
tuple	reflect(tuple in, tuple normal);

// render.c

tuple	lighting(material m, shape object, light l, tuple p, tuple eyev, tuple normalv, bool in_shadow);
bool is_shadowed(world w, tuple p, light l);
tuple		shade_hit(world w, computation c, int remaining);
tuple	color_at(world w, ray r, int remaining);
void	prepare_computations(computation *comps, ray r, intersections xs);
tuple ray_for_pixel(camera cam, matrix *inv, tuple origin, int px, int py);
canvas render(camera cam, world w);
canvas low_render(camera cam, world w, int step);
tuple reflected_color(world w, computation comps, int remaining);
void	printf_time(clock_t start, clock_t end);

// sphere.c

shape	create_sphere(void);
intersections sphere_intersect(shape *s, ray r);
tuple	sphere_normal_at(shape *s, tuple world_point);

// plane.c

shape	create_plane(void);
intersections	plane_intersect(shape *p, ray r);
tuple	plane_normal_at(shape *p, tuple world_point);

// cube.c

shape create_cube(void);
tuple cube_normal_at(shape *cube, tuple object_point);
intersections cube_intersect(shape *cube, ray local_ray);

// cylinder.c
shape create_cylinder(void);
intersections cylinder_intersect(shape *s, ray local_ray);
tuple cylinder_normal_at(shape *s, tuple object_point);

// cone.c
shape create_cone(void);
tuple cone_normal_at(shape *s, tuple p);
intersections cone_intersect(shape *cone, ray r);

// patterns.c

pattern	stripe_pattern(tuple a, tuple b);
tuple	stripe_at(pattern *s, tuple p);
tuple	pattern_at_object(pattern *p, shape s, tuple world_point);
pattern	gradient_pattern(tuple a, tuple b);
tuple	gradient_at(pattern *p, tuple point);
pattern	ring_pattern(tuple a, tuple b);
tuple	ring_at(pattern *p, tuple point);
pattern	checker_pattern(tuple a, tuple b);
tuple	checker_at(pattern *p, tuple point);

//	scenes

canvas simple(int width, int height, int fov, int step);
canvas test(int	width, int height, int fov, int step);
canvas first_scene(int	width, int height, int fov, int step);
canvas color_scene(int width, int height, int fov);


// material.c 

material create_glass_material(void);
material create_mirror_material(void);
material create_matte_material(tuple color);
material create_metal_material(tuple color);
material	create_material(void);

// threads.c
canvas thread_render(camera cam, world w);

// utils.c
typedef struct
{
    struct timespec start;
} timer;

void start_timer(timer *t);
void print_elapsed_time(timer *t);

#endif