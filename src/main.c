#include "raytracing.h"

#include <assert.h>


void test_intersect_scaled_sphere(void)
{
    ray r = create_ray(point(0, 0, -5), vector(0, 0, 1));
    sphere s = create_sphere();

    // Apply scaling transformation (scaling by 2 in all axes)
    matrix m = scaling(2, 2, 2);
    set_transform(&s, m);  // Assuming set_transform function exists and updates sphere's transformation

    intersections xs = intersect(s, r);

    // Assert the intersection results
    assert(xs.count == 2);
    assert(fabs(xs.list[0].t - 3.0) < 0.0001);  // Using epsilon for floating-point comparison
    assert(fabs(xs.list[1].t - 7.0) < 0.0001);
}

void test_intersect_translated_sphere(void)
{
    ray r = create_ray(point(0, 0, -5), vector(0, 0, 1));
    sphere s = create_sphere();

    // Apply translation transformation (move sphere by (5, 0, 0))
    matrix m = translation(5, 0, 0);
    set_transform(&s, m);  // Assuming set_transform function exists and updates sphere's transformation

    intersections xs = intersect(s, r);

    // Assert the intersection results
    assert(xs.count == 0);
}

int main(void)
{
    // Run the tests
    test_intersect_scaled_sphere();
    test_intersect_translated_sphere();

    printf("All tests passed successfully!\n");
    return 0;
}
