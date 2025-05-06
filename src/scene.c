#include "raytracing.h"

world	create_world();
{
	world w;

	return (w);
}

world default_world(void)
{
    world w;
    
    // Create a light source
    w.light_source = point_light(point(-10, 10, -10), color(1, 1, 1));
    
    // Allocate memory for 2 spheres
    w.object_count = 2;
    w.objects = malloc(sizeof(sphere) * w.object_count);
    
    // First sphere: default material and transformation
    w.objects[0] = create_sphere(); // Using the create_sphere function from your previous code
    w.objects[0].material.color = color(0.8, 1.0, 0.6); // Example color
    w.objects[0].material.diffuse = 0.7;
	w.objects[0].material.specular = 0.2;
    
    // Second sphere: scaled, with a different material
    w.objects[1] = create_sphere();
    set_transform(&w.objects[1], scaling(0.5, 0.5, 0.5)); // Scale the second sphere
    
    return w;
}

intersections intersect_world(world w, ray r)
{
    intersections xs;
    xs.count = 0;

    // Intersect the ray with each object in the world
    for (int i = 0; i < w.object_count; i++)
    {
        intersections temp = intersect(w.objects[i], r);
        for (int j = 0; j < temp.count; j++)
        {
            xs.list[xs.count++] = temp.list[j];
        }
    }
    return xs;
}