#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "barrier.h"

#define NUM_PROCESSES  8

// Simple function to check if a number is prime
// If number is not even, iterates through odd numbers until finding a divisor
// or stops once it crosses the square root of the number
// Leave this as is
int isPrime(int input) {
    if (input % 2 == 0) return 0;
    for (int i = 3; i*i <= input; i+=2) {
        if (input % i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {

    int pid, shmid1;
    int *counts, *vect;
    int input_size;
    double time_taken;
    int per_process;

    struct timespec startTime, endTime;

    scanf("%d", &input_size);
    if(input_size % NUM_PROCESSES != 0) {
        printf("Vector size of %d is not divisible by %d processes.\n", input_size, NUM_PROCESSES);
        exit(-1);
    }
    per_process = input_size / NUM_PROCESSES;

    int i;
    vect = (int *) malloc(input_size * sizeof(int));
    for(i=0; i<input_size; i++)
        scanf("%d", vect+i);

    shmid1 = shmget(IPC_PRIVATE, NUM_PROCESSES * sizeof(int), IPC_CREAT | 0600);
    counts = (int *) shmat(shmid1, NULL, 0);

    init_barrier(NUM_PROCESSES);

    for (i = 0; i < NUM_PROCESSES; i++)
        counts[i] = 0;

    for(i=0; i<NUM_PROCESSES-1; i++) {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
    }
    int j;
    int totalCounts = 0;

    int start = i * per_process;
    int end = (i+1) * per_process;

    clock_gettime(CLOCK_MONOTONIC, &startTime);
    for (j = start; j < end; j++)
    {
        if (isPrime(vect[j]))
            counts[i]++;
    }

    reach_barrier();

    if (pid != 0)
    {
        for(j=0; j<NUM_PROCESSES; j++)
        {
            totalCounts += counts[j];
        }
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        time_taken = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1e9;

        printf("\nNumber of items: %d\n", input_size);
        printf("Number of primes is %d\n", totalCounts);
        printf("Time taken is %3.3f seconds\n\n", time_taken);

        // Clean up process table
        for(j=0; j<NUM_PROCESSES; j++)
            wait(NULL);

        shmdt(counts);
        shmctl(shmid1, IPC_RMID, 0);
        destroy_barrier(pid);
    }
}