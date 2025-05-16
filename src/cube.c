#include "raytracing.h"

shape create_cube(void)
{
    static int id = 0;
    shape c;

    c.type = SHAPE_CUBE;
    c.id = id++;
    set_transform(&c, matrix_identity(4));
    c.material = create_material();
    c.intersect = &cube_intersect;
    c.normal_at = &cube_normal_at;

    return (c);
}

tuple cube_normal_at(shape *cube, tuple object_point)
{
    (void)cube;

    float abs_x = fabsf(object_point.x);
    float abs_y = fabsf(object_point.y);
    float abs_z = fabsf(object_point.z);
    float maxc = fmaxf(abs_x, fmaxf(abs_y, abs_z));

    if (equal(maxc, abs_x))
        return vector(object_point.x, 0, 0);
    else if (equal(maxc, abs_y))
        return vector(0, object_point.y, 0);
    else
        return vector(0, 0, object_point.z);
}

static void check_axis(float origin, float direction, float *tmin, float *tmax)
{
    float tmin_numerator = -1.0f - origin;
    float tmax_numerator = 1.0f - origin;

    float tmin_temp, tmax_temp;

    if (fabsf(direction) >= EPSILON)
    {
        tmin_temp = tmin_numerator / direction;
        tmax_temp = tmax_numerator / direction;
    }
    else
    {
        tmin_temp = tmin_numerator * INFINITY;
        tmax_temp = tmax_numerator * INFINITY;
    }

    if (tmin_temp > tmax_temp)
    {
        float tmp = tmin_temp;
        tmin_temp = tmax_temp;
        tmax_temp = tmp;
    }

    *tmin = tmin_temp;
    *tmax = tmax_temp;
}

intersections cube_intersect(shape *cube, ray local_ray)
{
    intersections result = {.count = 0};

    float xtmin, xtmax;
    float ytmin, ytmax;
    float ztmin, ztmax;

    check_axis(local_ray.origin.x, local_ray.direction.x, &xtmin, &xtmax);
    check_axis(local_ray.origin.y, local_ray.direction.y, &ytmin, &ytmax);
    check_axis(local_ray.origin.z, local_ray.direction.z, &ztmin, &ztmax);

    float tmin = fmaxf(fmaxf(xtmin, ytmin), ztmin);
    float tmax = fminf(fminf(xtmax, ytmax), ztmax);

    if (tmin > tmax)
        return (result); // No intersection

    result.list[0] = create_intersection(tmin, *cube);
    result.list[1] = create_intersection(tmax, *cube);
    result.count = 2;

    return (result);
}
