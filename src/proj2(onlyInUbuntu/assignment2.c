#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sharedMemory.h"
#include "semaphore.h"
#define SHMKEY ((key_t) 1497) // macro to define shared memory

int shmid;

int main() {
    int pid1, pid2, pid3, pid4;
    char *shmadd;
    shmadd = (char *) 0;

    initialize_shared_memory();
    total->value = 0;

    int value, value1;
    semunion semctl_arg;
    semctl_arg.val = 1;

    /* Create semaphores */
    sem_id = semget(SEMKEY, NSEMS, IPC_CREAT | 0666);
    if(sem_id < 0) printf("Error in creating the semaphore./n");
    /* Initialize semaphore */
    value1 =semctl(sem_id, NSEMS, SETVAL, semctl_arg);
    value =semctl(sem_id, NSEMS, GETVAL, semctl_arg);
    if (value < 0) printf("Error detected in SETVAL.\n");



    (pid1 = fork()) == 0 ? (createProcessAfter(total->value, 100000, '1'), exit(0)) : 0;
    (pid2 = fork()) == 0 ? (createProcessAfter(total->value, 200000, '2'), exit(0)) : 0;
    (pid3 = fork()) == 0 ? (createProcessAfter(total->value, 300000, '3'), exit(0)) : 0;
    (pid4 = fork()) == 0 ? (createProcessAfter(total->value, 500000, '4'), exit(0)) : 0;
    //create all child processes

    waitForChildren();

    /* De-allocate semaphore */
    semctl_arg.val = 0;
    int status = semctl(sem_id, 0, IPC_RMID, semctl_arg);
    if(status < 0) printf("Error in removing the semaphore.\n");

    detach_and_cleanup_shared_memory();
    printf("End of Simulation.");

    return 0;
}

void detach_and_cleanup_shared_memory() {
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }
    shmctl(shmid, IPC_RMID, NULL); // detach and delete shared memory
}

void waitForChildren() {
    for (int i = 0; i < 4; i++) {
        int status;
        pid_t child_pid = wait(&status);
        printf("Child with ID: %d has just exited.\n", child_pid);
    }
}

void initialize_shared_memory() {
    if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((total = (shared_mem *) shmat(shmid, NULL, 0)) == (shared_mem *) -1) {
        perror("shmat");
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
 * @param processNum Current process number assigned by usr
 * @return updated total value and a print of process counter
 */
int createProcessAfter(int total, int valTo, char processNum) {
    POP();
    total = incrementByOne(total, valTo);
    printf("From Process %c: counter = %d\n", processNum, total);
    VOP();
    return total;
}

