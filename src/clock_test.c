#include "raytracing.h"


void draw_clock_face(void)
{
    int width = 200;
    int height = 200;
    float radius = 90.0f;
    canvas c = create_canvas(width, height);
    tuple red = color(1.0f, 0.0f, 0.0f);

    for (int hour = 0; hour < 12; ++hour)
    {
        float angle = (30) * hour; // 30 degrees per hour
        matrix rot = rotation_z(angle); 
        tuple pos = matrix_multiply_tuple(rot, point(0, 1, 0));

        int x = (int)(width / 2 + pos.x * radius);
        int y = (int)(height / 2 + pos.y * radius);

        write_pixel(&c, x, y, red);
    }

    canvas_to_ppm(&c, "clock.ppm");
    destroy_canvas(&c);
}

int main(void)
{
    draw_clock_face();
    return (0);
}