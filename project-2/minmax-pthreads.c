// compile with: gcc -o minmax-pthreads minmax-pthreads.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N 100000000
#define NUM_THREADS 8

struct ThreadData
{
    double *a;
    double maxval;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void get_walltime(double *wct)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}

void *find_max(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    double *a = data->a;
    double maxval = data->maxval;

    size_t start = 0;
    size_t end = N;

    for (size_t i = start; i < end; i++)
    {
        if (a[i] > maxval)
        {
            maxval = a[i];
        }
    }

    pthread_mutex_lock(&mutex);
    if (maxval > data->maxval)
    {
        data->maxval = maxval;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    double ts, te;
    double *a;

    // allocate array
    a = (double *)malloc(N * sizeof(double));
    if (a == NULL)
    {
        printf("Allocation failed!\n");
        exit(1);
    }

    // fill array with random numbers
    srand(0);
    a[0] = (double)rand() / RAND_MAX;
    double checkmax = a[0];
    for (size_t i = 1; i < N; i++)
    {
        a[i] = (double)rand() / RAND_MAX;
        if (a[i] > checkmax)
        {
            checkmax = a[i];
        }
    }

    // get starting time (double, seconds)
    get_walltime(&ts);

    // max reduction
    double maxval = a[0];

    pthread_t threads[NUM_THREADS];
    struct ThreadData thread_data[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].a = a;
        thread_data[i].maxval = maxval;
    }

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&threads[i], NULL, find_max, (void *)&thread_data[i]) != 0)
        {
            printf("Error creating thread!\n");
            exit(1);
        }
    }

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            printf("Error joining thread!\n");
            exit(1);
        }
    }

    maxval = thread_data[0].maxval;

    for (size_t i = 1; i < NUM_THREADS; i++)
    {
        if (thread_data[i].maxval > maxval)
        {
            maxval = thread_data[i].maxval;
        }
    }

    // get ending time
    get_walltime(&te);

    // check result
    if (maxval != checkmax)
    {
        printf("Reduction error!\n");
    }

    // free array
    free(a);

    // destroy mutex
    pthread_mutex_destroy(&mutex);

    printf("Exec Time (sec) = %f\n", te - ts);

    return 0;
}
