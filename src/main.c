
#include <stdio.h>
#include <math.h>
#include "raytracing.h"  // adjust this if your header file is named differently

void    test_lighting_with_light_behind_surface(void)
{
    material m = create_material();
    tuple position = point(0, 0, 0);
    tuple eyev = vector(0, 0, -1);
    tuple normalv = vector(0, 0, -1);
    light l = point_light(point(0, 0, 10), color(1, 1, 1));

    tuple result = lighting(m, l, position, eyev, normalv);
    tuple expected = color(0.1, 0.1, 0.1);
    if (!equal_tuple(result, expected))
        printf("FAIL: light behind surface, got (%.4f %.4f %.4f)\n", result.x, result.y, result.z);
    else
        printf("PASS: light behind surface\n");
}

void    test_lighting_with_eye_in_reflection_vector(void)
{
    material m = create_material();
    tuple position = point(0, 0, 0);
    tuple eyev = vector(0, -sqrtf(2)/2, -sqrtf(2)/2);
    tuple normalv = vector(0, 0, -1);
    light l = point_light(point(0, 10, -10), color(1, 1, 1));

    tuple result = lighting(m, l, position, eyev, normalv);
    tuple expected = color(1.6364, 1.6364, 1.6364);
    if (!equal_tuple(result, expected))
        printf("FAIL: eye in reflection vector, got (%.4f %.4f %.4f)\n", result.x, result.y, result.z);
    else
        printf("PASS: eye in reflection vector\n");
}

void    test_lighting_with_eye_opposite_surface_and_light_offset(void)
{
    material m = create_material();
    tuple position = point(0, 0, 0);
    tuple eyev = vector(0, 0, -1);
    tuple normalv = vector(0, 0, -1);
    light l = point_light(point(0, 10, -10), color(1, 1, 1));

    tuple result = lighting(m, l, position, eyev, normalv);
    tuple expected = color(0.7364, 0.7364, 0.7364);
    if (!equal_tuple(result, expected))
        printf("FAIL: eye opposite, light 45°, got (%.4f %.4f %.4f)\n", result.x, result.y, result.z);
    else
        printf("PASS: eye opposite, light 45°\n");
}


int	main(void)
{
	test_lighting_with_eye_opposite_surface_and_light_offset();
	test_lighting_with_eye_in_reflection_vector();
	test_lighting_with_light_behind_surface();
	return (0);
}