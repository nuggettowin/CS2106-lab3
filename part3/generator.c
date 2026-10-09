#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 2000000

int isPrime(int input) {
    if (input % 2 == 0) return 0;
    for (int i = 3; i*i <= input; i+=2) {
        if (input % i == 0) return 0;
    }
    return 1;
}

int compare_integers(const void *a, const void *b) {
	int int_a = *((int*)a);
    int int_b = *((int*)b);

    if (int_a == int_b) return 0;
    else if (int_a < int_b) return 1;
    else return -1;
}

void create_first() {
    int created = 0;
    FILE *fptr = fopen("test1.in", "w");
	int *data = (int *) malloc(sizeof(int) * SIZE);
    if (fptr == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    fprintf(fptr, "%d\n", SIZE);
    while (created < SIZE) {
        while (1) {
            int num = rand();
            if (num < 1 << 10 || num %2 == 0) continue;
            else {
                data[created] = num;
                created++;
                break;
            }
        }
    }
	qsort(data, SIZE, sizeof(int), compare_integers);
	for (int i = 0; i < SIZE; i++) {
		fprintf(fptr, "%d\n", data[i]);
	}
}

void create_second() {
    int created = 0;
    FILE *fptr = fopen("test2.in", "w");
	int *data = (int *) malloc(sizeof(int) * SIZE);
    if (fptr == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    fprintf(fptr, "%d\n", SIZE);
    while (created < SIZE)
    {
        while (1)
        {
            int num = rand();
            if (!isPrime(num))
            {
                
                if (rand() % 3 == 0 && num %2 != 0)
                {
                    data[created] = num;
                    created++; 
                    break;
                }
            }
            else
            {
                data[created] = num;
                created++;
                break;
            }
        }
    }
	qsort(data, SIZE, sizeof(int), compare_integers);
	for (int i = 0; i < SIZE; i++) {
		fprintf(fptr, "%d\n", data[i]);
	}
}

int main()
{
    srand(42);
    create_first();
    create_second();
    return 0;
}