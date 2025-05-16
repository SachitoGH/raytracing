#include "raytracing.h"

void start_timer(timer *t)
{
    clock_gettime(CLOCK_MONOTONIC, &t->start);
}

void print_elapsed_time(timer *t)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    double elapsed = (now.tv_sec - t->start.tv_sec) +
                     (now.tv_nsec - t->start.tv_nsec) / 1e9;

    printf("Elapsed time: %.6f seconds\n", elapsed);
}