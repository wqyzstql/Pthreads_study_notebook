#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>

using namespace std;

constexpr int NUM_THREADS = 3;
int thread_count;

const int N = 100;

int n;
int A[N][N], x[N], y[N];

static void* hello(void *rank) {
    long long my_rank = (long)rank;
    int start = my_rank * ((static_cast<long long>(n) + thread_count - 1) / thread_count);
    int end = (my_rank + 1) * ((static_cast<long long>(n) + thread_count - 1) / thread_count) - 1;
    //printf("Hello from thread %lld start=%d end=%d\n", my_rank,start,end);
    for (int i = start; i <= min(n-1, end); i++) {
        for (int j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
    return NULL;
}
int main(int argc, char *argv[]) {
    thread_count = NUM_THREADS;
    vector<pthread_t> thread_handles(thread_count);
    n = 7;
    for (int i = 0; i < n; i++)for (int j = 0; j < n; j++)A[i][j] = i + j;
    for (int i = 0; i < n; i++) x[i] = i;

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, hello, (void*)i);// index 
    }
    //printf("Hello from the main thread\n");
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);// stop the thread
    }//also can dont stop the thread and assign new task for it (4.5)
    for (int i = 0; i < n; i++)    cout << y[i] << " ";
    return 0;
}
