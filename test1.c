#include "raytracing.h"

typedef struct projectile
{
	tuple	pos;
	tuple	velocity;
}	projectile;

typedef struct environement
{
	tuple	gravity;
	tuple	wind;
}	environment;

projectile	tick(environment env, projectile proj)
{
	projectile	ret;

	ret.pos = add_tuple(proj.pos, proj.velocity);
	ret.velocity = add_tuple(add_tuple(proj.velocity, env.gravity), env.wind);
	return (ret);
}

int main(void) {
    // Initialize canvas with width=200 and height=100
    canvas c = create_canvas(200, 100);

    // Define projectile and environment
    projectile p = {point(0, 1, 0), vector(1, 1, 0)};
    environment e = {vector(0, -0.1, 0), vector(-0.01, 0, 0)};
    tuple red = color(1.0f, 0.0f, 0.0f);

    // Simulate projectile motion and plot points
    int tick_count = 0;
    while (p.pos.y > 0 && tick_count < 100) {
        // Map the x, y coordinates to the canvas dimensions
        int x = (int)(p.pos.x * 10);  // Scale the x-axis for canvas width
        int y = (int)(p.pos.y * 10);  // Scale the y-axis for canvas height

        // Write red pixel at the corresponding position
        write_pixel(&c, x, 100 - y, red);  // Adjust to fit canvas

        // Update the projectile
        p = tick(e, p);
        tick_count++;
    }

    // Save the trajectory as a PPM file
    canvas_to_ppm(&c, "projectile_trajectory.ppm");

    // Clean up
    destroy_canvas(&c);
    return 0;
}