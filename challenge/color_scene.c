#include "raytracing.h"

canvas color_scene(int width, int height, int fov) // WORK IN PROGRESS
{
    world w;
    matrix temp1, temp2;

    // Create three colored light sources for flower-like shadow
    w.light_count = 3;
    w.lights = malloc(sizeof(light) * w.light_count);
    w.lights[0] = point_light(point(-3.0f, 4.0f, -4.0f), color(1.0f, 0.3f, 0.3f)); // Reddish light
    w.lights[1] = point_light(point(0.0f, 4.0f, -4.0f), color(0.3f, 1.0f, 0.3f)); // Greenish light
    w.lights[2] = point_light(point(3.0f, 4.0f, -4.0f), color(0.3f, 0.3f, 1.0f)); // Bluish light

    // Allocate memory for 1 sphere and 2 planes
    w.object_count = 3;
    w.objects = malloc(sizeof(shape) * w.object_count);

    // Reflective sphere in the air
    w.objects[0] = create_sphere();
    temp1 = scaling(1.0f, 1.0f, 1.0f);
    temp2 = translation(0.0f, 2.0f, 0.0f); // Floating 2 units above floor
    set_transform(&w.objects[0], matrix_multiply(&temp1, &temp2));

    // Wall plane to catch flower-like shadow
    w.objects[1] = create_plane();
    w.objects[1].material.color = color(0.9f, 0.9f, 0.9f); // Light gray for shadow visibility
    w.objects[1].material.diffuse = 0.8f; // Soft lighting
    w.objects[1].material.specular = 0.0f; // No highlights to focus on shadows
    temp1 = translation(0.0f, 0.0f, 5.0f); // Positioned 5 units behind sphere
    temp2 = rotation_x(90.0f); // Vertical wall
    set_transform(&w.objects[1], matrix_multiply(&temp1, &temp2));

    // Floor plane with subtle checker pattern
    w.objects[2] = create_plane();
    w.objects[2].material.color = color(1.0f, 1.0f, 1.0f);
    w.objects[2].material.pattern = checker_pattern(color(0.8f, 0.8f, 0.8f), color(0.6f, 0.6f, 0.6f)); // Subtle gray checkers
    w.objects[2].material.reflective = 0.1f; // Slight reflection for realism
    set_transform(&w.objects[2], translation(0.0f, -1.0f, 0.0f)); // Flat floor

    // Camera to capture sphere and shadow
    camera cam = create_camera(width, height, fov);
    set_camera_transform(&cam, view_transform(
        point(0.0f, 2.0f, -8.0f), // Aligned with sphere height, back 8 units
        point(0.0f, 2.0f, 0.0f),  // Looking at sphere center
        vector(0.0f, 1.0f, 0.0f)  // Up vector
    ));

    // Render with thread_render for speed
    canvas image = thread_render(cam, w);

    // Clean up
    destroy_world(&w);
    return image;
}
