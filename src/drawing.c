#include "raytracing.h"

canvas create_canvas(int width, int height)
{
    canvas c;
    c.width = width;
    c.height = height;
    c.pixels = (tuple*)malloc(width * height * sizeof(tuple));

    // Initialize all pixels to black (0, 0, 0)
    for (int i = 0; i < width * height; ++i) {
        c.pixels[i] = vector(0.0f, 0.0f, 0.0f);
    }
    return c;
}

void destroy_canvas(canvas* c)
{
    free(c->pixels);
    c->pixels = NULL;
}

tuple pixel_at(canvas* c, int x, int y)
{
    return c->pixels[y * c->width + x];
}

void write_pixel(canvas* c, int x, int y, tuple col)
{
    if (x < 0 || x >= c->width || y < 0 || y >= c->height)
		return;
    c->pixels[y * c->width + x] = col;
}

tuple	color(float r, float g, float b)
{
	return ((tuple){r, g, b, 0.0});
}

int to_ppm_component(float value)
{
    int scaled = (int)roundf(value * 255.0f);
    if (scaled < 0)
		return (0);
    if (scaled > 255) 
		return (255);
    return (scaled);
}

void canvas_to_ppm(canvas* c, const char* filename)
{
    FILE* f = fopen(filename, "w");
    if (!f)
		return;

    // Header
    fprintf(f, "P3\n%d %d\n255\n", c->width, c->height);

    // Pixel data
    for (int y = 0; y < c->height; ++y)
	{
        int line_length = 0;
        for (int x = 0; x < c->width; ++x)
		{
            tuple col = pixel_at(c, x, y);
            int r = to_ppm_component(col.x);
            int g = to_ppm_component(col.y);
            int b = to_ppm_component(col.z);

            // Write RGB values, respecting 70-char line length
            int needed = snprintf(NULL, 0, "%d %d %d ", r, g, b);
            if (line_length + needed > 70)
			{
                fprintf(f, "\n");
                line_length = 0;
            }

            fprintf(f, "%d %d %d ", r, g, b);
            line_length += needed;
        }
        fprintf(f, "\n");
    }
    fclose(f);
}