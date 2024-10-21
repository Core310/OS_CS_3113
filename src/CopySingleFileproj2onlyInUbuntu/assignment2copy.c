#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHMKEY ((key_t) 1497) // macro to define shared memory
#define SEMKEY ((key_t) 1498) // macro to define semaphore key

/**
 * Struct for shared memory, bound to variable total
 */
typedef struct {
    int value;
} shared_mem;
shared_mem *total;

/* Semaphore union used to initialize semaphore */
typedef union {
    int val;
    struct semid_ds *buf;
    ushort *array;
} semunion;

int sem_id;  // semaphore id

/* semaphore buffers for P (wait) and V (signal) operations */
static struct sembuf OP = {0, -1, 0};  // P operation (wait)
static struct sembuf OV = {0, 1, 0};   // V operation (signal)
struct sembuf *P = &OP;
struct sembuf *V = &OV;

/* Semaphore wait operation */
void semaphore_wait() {
    if (semop(sem_id, P, 1) == -1) {
        perror("semop P");
        exit(1);
    }
}

/* Semaphore signal operation */
void semaphore_end_signal() {
    if (semop(sem_id, V, 1) == -1) {
        perror("semop V");
        exit(1);
    }
}

/**
 * Increments input value (total) to a new declared value, child method to createProcessAfter
 * @param oldVal total global variable
 * @param newVal new value to assign to total
 * @return
 */
int incrementByOne(int oldVal, int newVal) {
    while (oldVal < newVal) oldVal++;
    return oldVal;
}

/**
 *
 * @param total input for incrementByOne, representing `total` global variable
 * @param valTo input for incrementByOne
 * @param processNum Current process number assigned by user
 * @return updated total value and a print of process counter
 */
int createProcessAfter(int total, int valTo, char processNum) {
    semaphore_wait();

    total = incrementByOne(total, valTo);
    printf("From Process %c: counter = %d\n", processNum, total);

    semaphore_end_signal();
    return total;
}

int main() {
    int shmid, pid1, pid2, pid3, pid4;
    char *shmadd;
    shmadd = (char *) 0;

    // make SHM
    if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((total = (shared_mem *) shmat(shmid, NULL, 0)) == (shared_mem *) -1) {
        perror("shmat");
        exit(1);
    }

    total->value = 0;  // Initialize shared memory variable

    // Make Semaphore
    sem_id = semget(SEMKEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    // Init semaphore
    semunion sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        perror("semctl");
        exit(1);
    }

    // Fork and create child processes
    (pid1 = fork()) == 0 ? (createProcessAfter(total->value, 100000, '1'), exit(0)) : 0;
    (pid2 = fork()) == 0 ? (createProcessAfter(total->value, 200000, '2'), exit(0)) : 0;
    (pid3 = fork()) == 0 ? (createProcessAfter(total->value, 300000, '3'), exit(0)) : 0;
    (pid4 = fork()) == 0 ? (createProcessAfter(total->value, 500000, '4'), exit(0)) : 0;

    // Wait for all child processes to complete
    for (int i = 0; i < 4; i++) {
        int status;
        pid_t child_pid = wait(&status);
        printf("Child with ID: %d has just exited.\n", child_pid);
    }

    // Detach and remove shared memory
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);  // Remove shared memory

    // -rm semaphore
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        perror("semctl IPC_RMID");
        exit(1);
    }

    printf("End of Simulation.\n");

    return 0;
}
