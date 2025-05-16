#include "raytracing.h"
#include <pthread.h>
#include <sys/time.h>

#include <pthread.h>

// Structure to pass data to threads
typedef struct 
{
    camera* cam;
    world* w;
    canvas* image;
    matrix* inv;
    int start_y;
    int end_y;
    int step; // Already added for interleaved rows
    tuple origin; // Precomputed ray origin
} thread_data;

// Thread function that renders a portion of the image
void* render_portion(void* arg) 
{
    thread_data* data = (thread_data*)arg;
    ray r;
    r.origin = data->origin; // Use precomputed origin
    
    for (int y = data->start_y; y < data->end_y; y += data->step) {
        for (int x = 0; x < data->cam->hsize; x++) {
            r.direction = ray_for_pixel(*(data->cam), data->inv, r.origin, x, y);
            write_pixel(data->image, x, y, color_at(*(data->w), r, 4));
        }
    }

    return NULL;
}

#include <sys/sysinfo.h>

canvas thread_render(camera cam, world w)
{
    canvas image = create_canvas(cam.hsize, cam.vsize);
    matrix inv = inverse(&cam.transform);

    int num_threads = get_nprocs(); // Dynamic thread count
    pthread_t threads[num_threads];
    thread_data thread_args[num_threads];
    
    timer t;
	start_timer(&t);
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].cam = &cam;
        thread_args[i].w = &w;
        thread_args[i].image = &image; // Fixed typo: ℑ → &image
        thread_args[i].inv = &inv;
        thread_args[i].start_y = i;
        thread_args[i].end_y = cam.vsize;
        thread_args[i].step = num_threads; // Interleaved rows
        thread_args[i].origin = matrix_multiply_tuple(&inv, point(0.0f, 0.0f, 0.0f)); // Precompute origin
        pthread_create(&threads[i], NULL, render_portion, &thread_args[i]);
    }
    
    for (int i = 0; i < num_threads; i++)
	{
        pthread_join(threads[i], NULL);
    }
	print_elapsed_time(&t);
	return image;
}