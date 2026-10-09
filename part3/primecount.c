#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int isPrime(int input) {
    if (input % 2 == 0) return 0;
    for (int i = 3; i*i <= input; i+=2) {
        if (input % i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {

    int input_size;
    int *vect;
    double time_taken;

    struct timespec start, end;

    scanf("%d", &input_size);

    int i;
    vect = (int *) malloc(input_size * sizeof(int));
    for(i=0; i<input_size; i++)
        scanf("%d", vect+i);

    int totalCounts = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < input_size; i++)
    {
        if (isPrime(vect[i]))
            totalCounts++;
    }
        
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nNumber of items: %d\n", input_size);
    printf("Number of primes is %d\n", totalCounts);
    printf("Time taken is %3.3f seconds\n\n", time_taken);
}