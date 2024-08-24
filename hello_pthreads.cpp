#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>

using namespace std;

#define NUM_THREADS 5
int thread_count;

void* hello(void *rank) {
    long long my_rank = (long)rank;
    printf("Hello from thread%lld\n", my_rank);
    return NULL;
}

int main(int argc, char *argv) {
    thread_count = NUM_THREADS;
    vector<pthread_t> thread_handles(thread_count);
    //thread_count = strtol(argv[i],NULL,10);
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, hello, (void*)i);
    }
    printf("Hello from the main thread\n");
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    return 0;
}
