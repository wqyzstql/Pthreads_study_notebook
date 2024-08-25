#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h> // sem_t
#include <vector>
#include <chrono>

using namespace std;

constexpr int NUM_THREADS = 8;
int thread_count;

long long n = 1000000000, sum = 0;
int counter = 0;
sem_t message ,cond_var;

static void* Monte(void *rank) {
    long long my_rank = (long)rank;
    long long local_sum = 0;
    long long stride = n / thread_count;
    long long local_start = (my_rank - 1) * stride;
    long long local_end = my_rank * stride - 1;
    for (long long i = local_start; i <= min(n, local_end); i++) {
        int x1 = rand() % 20, x2 = rand() % 20;
        double x = 1.0 * x1 / 10 - 1, y = 1.0 * x2 / 10 - 1;//-1,1
        double dist = x * x + y * y;
        if (dist <= 1) {
            local_sum++;
        }
    }
    sem_wait(&message);
    sum += local_sum;
    sem_post(&message);
    return NULL;
}
int main(int argc, char *argv[]) {
    srand((unsigned)(long long)argv);
    sem_init(&message, 0, 1);// once set a value
    thread_count = NUM_THREADS;
    vector<pthread_t> thread_handles(thread_count);
    auto tstart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, Monte, (void*)i);// index 
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);// stop the thread
    }
    //cout << 4.0 * sum / n << endl;
    auto tend = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_sycl = tend - tstart;
    std::cout << "Pthreads code time: " << duration_sycl.count() << " seconds" << std::endl;
    return 0;
}
