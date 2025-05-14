#include "raytracing.h"

void	*canvas_to_img(canvas *c, void *mlx)
{
	void	*img = mlx_new_image(mlx, c->width, c->height);
	int		endian, bits_per_pixel, line_len;
	char	*addr = mlx_get_data_addr(img, &bits_per_pixel, &line_len, &endian);

	for (int i = 0; i < c->height; i++)
	{
		for (int j = 0; j < c->width; j++)
		{
			tuple col = pixel_at(c, j, i);
            int r = to_ppm_component(col.x);
            int g = to_ppm_component(col.y);
            int b = to_ppm_component(col.z);
			*(unsigned int *)(addr + (i * line_len
				+ j * (bits_per_pixel / 8))) = (r << 16) | (g << 8) | b;
		}
	}
	return (img);
}

int	end(int key, void *mlx)
{
	if (key == ESC_KEY)
		mlx_loop_end(mlx);
	return (0);
}

int main(void)
{
	int		width	= 1440;
	int		height	= 768;
    canvas	c		= simple(width, height, 70, 1);
	void	*mlx	= mlx_init();
	void	*win	= mlx_new_window(mlx, width, height, "MiniRT");
	void	*img	= canvas_to_img(&c, mlx);

	destroy_canvas(&c);
	mlx_put_image_to_window(mlx, win, img, 0, 0);
	mlx_key_hook(win, end, mlx);
	mlx_loop(mlx);
	mlx_destroy_image(mlx, img);
	mlx_destroy_window(mlx, win);
	mlx_destroy_display(mlx);
	free(mlx);
    return 0;
}

/*int main(void)
{
	pattern test = ring_pattern(color(1, 1, 1), color(0,0,0));

	print_tuple(test.pattern_at(&test, point(0.7,0,0.7)));
	return(0);
}*/