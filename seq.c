#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <unistd.h>

int X = 0;
int Y = 0;
int x = 0;
int y = 0;
sem_t SX;
sem_t SXX;
sem_t SY;
sem_t SYY;

void* threadX(void* param) {
    // Set thread affinity to CPU 10
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(10, &cpus);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpus);
    while(true) {
        sem_wait(&SX);
        // LOAD & STORE
        X = 1;
        asm volatile("" ::: "memory");
        x = Y;
        sem_post(&SXX);
    }
    return NULL;
}

void* threadY(void* param) {
    // Set thread affinity to CPU 1
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(1, &cpus);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpus);
    while(true) {
        sem_wait(&SY);
        // LOAD & STORE
        Y = 1;
        asm volatile("" ::: "memory");
        y = X;
        sem_post(&SYY);
    }
    return NULL;
}

int main() {
    long long int iter = 0;
    // Init Semaphores
    sem_init(&SX, 0, 0);
    sem_init(&SY, 0, 0);
    sem_init(&SXX, 0, 0);
    sem_init(&SYY, 0, 0);

    // Start threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, threadX, NULL);
    pthread_create(&thread2, NULL, threadY, NULL);

    // Check for Seq-Consistency
    while(++iter) {
        X = 0;
        Y = 0;
        sem_post(&SX);
        sem_post(&SY);
        sem_wait(&SXX);
        sem_wait(&SYY);
        if(x == 0 && y == 0) {
            printf("REORDERING DETECTED AFTER '%d' ITERATIONS\n", iter);
        }
    }
    return 0;
}
