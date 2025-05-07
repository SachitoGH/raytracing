#include "raytracing.h"

bool is_shadowed(world w, tuple p, light l)
{
    tuple direction = sub_tuple(l.position, p);  // Vector from point to light
    float distance = magnitude(direction);  // Distance from the point to the light
    direction = normalize(direction);  // Normalize the direction vector

    // Create a ray from the point to the light
    ray r = create_ray(p, direction);

    // Check for intersections with objects in the world
    intersections xs = intersect_world(w, r);

    // If there's an intersection and it's closer than the light source, the point is in shadow
    intersection* first = hit(&xs);
    if (first != NULL && first->t < distance)
    {
        return true; // Point is in shadow for this light
    }
    return false; // No shadow
}
