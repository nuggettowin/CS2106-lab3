#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>


int *count;
int nproc;
int shmid_barrier;
int shmid_mutex;
int shmid_count;
sem_t *barrier;
sem_t *mutex;

void init_barrier(int numproc) {
    nproc = numproc;

    shmid_count = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    count = shmat(shmid_count, NULL, 0);
    count[0] = 0;

    shmid_barrier = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    barrier = (sem_t *) shmat(shmid_barrier, NULL, 0);

    shmid_mutex = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    mutex = (sem_t *) shmat(shmid_mutex, NULL, 0);

    sem_init(barrier, 1, 0);
    sem_init(mutex, 1, 1);
}

void reach_barrier() {
    int done = 0;
    sem_wait(mutex);
    count[0]++;
    if (count[0] == nproc) {
        done = 1;
    }
    sem_post(mutex);
    if (done == 1) {
        sem_post(barrier);
    } else {
        sem_wait(barrier);
        sem_post(barrier);
    }
}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(barrier);
        sem_destroy(mutex);
        shmctl(shmid_count, IPC_RMID, 0);
        shmctl(shmid_barrier, IPC_RMID, 0);
        shmctl(shmid_mutex, IPC_RMID, 0);
    }
}