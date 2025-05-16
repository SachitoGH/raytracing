#include "raytracing.h"
#include <pthread.h>
#include <sys/time.h>

#include <pthread.h>

// Structure to pass data to threads
typedef struct {
    camera* cam;
    world* w;
    canvas* image;
    matrix* inv;
    int start_y;
    int end_y;
} thread_data;

// Thread function that renders a portion of the image
void* render_portion(void* arg) {
    thread_data* data = (thread_data*)arg;
    ray r;
    r.origin = matrix_multiply_tuple(data->inv, point(0.0f, 0.0f, 0.0f));
    
    for (int y = data->start_y; y < data->end_y; y++) {
        for (int x = 0; x < data->cam->hsize; x++) {
            r.direction = ray_for_pixel(*(data->cam), data->inv, r.origin, x, y);
            write_pixel(data->image, x, y, color_at(*(data->w), r, 4));
        }
    }
    
    return (NULL);
}

// Multi-threaded render function
canvas thread_render(camera cam, world w) {
    canvas image = create_canvas(cam.hsize, cam.vsize);
    matrix inv = inverse(&cam.transform);

    // Determine number of threads (adjust based on your CPU)
    int num_threads = 8; // Consider using number of CPU cores
    pthread_t threads[num_threads];
    thread_data thread_args[num_threads];
    
    // Calculate rows per thread
    int rows_per_thread = cam.vsize / num_threads;
    
	struct timeval start, end;
	gettimeofday(&start, NULL); 
    // Create and start threads
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].cam = &cam;
        thread_args[i].w = &w;
        thread_args[i].image = &image;
        thread_args[i].inv = &inv;
        thread_args[i].start_y = i * rows_per_thread;
        
        // Handle the case where vsize is not perfectly divisible by num_threads
        if (i == num_threads - 1) {
            thread_args[i].end_y = cam.vsize; // Last thread takes remaining rows
        } else {
            thread_args[i].end_y = (i + 1) * rows_per_thread;
        }
        
        pthread_create(&threads[i], NULL, render_portion, &thread_args[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
	gettimeofday(&end, NULL);
	printf("Time: %lis\n", end.tv_sec - start.tv_sec);
    return image;
}