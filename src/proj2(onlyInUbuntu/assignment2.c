#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"
#include "sharedMemory.h"
#define SHMKEY ((key_t) 1497) // macro to define shared memory

char *shmadd;
int shmid;
int value, value1;


void initialize_semaphore(int value, int value1, semunion semctl_arg) {
    value1 = semctl(sem_id, NSEMS, SETVAL, semctl_arg);
    value =semctl(sem_id, NSEMS, GETVAL, semctl_arg);
    if (value < 0) printf("Error detected in SETVAL.\n");
}//TODO 14/10/2024 value1 is never used! what is it for?

void De_allocate_semaphore(semunion *semctl_arg) {
    semctl_arg->val = 0;
    int status = semctl(sem_id, 0, IPC_RMID, *semctl_arg);
    if(status < 0) printf("Error in removing the semaphore.\n");
}

/*
* In this assignment, a memory location is shared by four processes. Each process independently
tries to increase the content of the shared memory location from 1 to a certain value by increments
of one. Process 1 has a target of 100000, Process 2’s target is 200000, Process 3 has a target of
300000, and the goal of Process 4 is 500000. When the program terminates, therefore, the shared
memory variable will have a total of 1100000 (i.e., this value will be printed by whichever of the
four processes finishes last).

In this project, you need to modify the code for programming assignment 1 to protect the critical
section using semaphores.

After all the children have finished, the parent process should release the shared memory and
semaphores and then terminate. Use the "wait" function so that the parent knows precisely when
each of the children finishes.  The parent should print the process ID of each child as the child
finishes execution. Then it should release shared memory, semaphores, and print "End of
Program".
 */

int main() {
    int pid1, pid2, pid3, pid4;
    initialize_shared_memory();
    total->value = 0;

    semunion semctl_arg;
    semctl_arg.val = 1;

    create_semaphores();
    initialize_semaphore(value, value1, semctl_arg);


    for (int i = 0; i < 4; i++) {
        int pid_status;
        pid_t child_pid = wait(&pid_status);
        printf("Child with ID: %d has just exited.\n", child_pid);
    }//wait for children to exit after kill
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }
    De_allocate_semaphore(&semctl_arg);
    shmctl(shmid, IPC_RMID, NULL);//detach and delete shared memory

    printf("End of Simulation.");

    return 0;
}


void create_semaphores(void) {
    /* Create semaphores */
    sem_id = semget(SEMKEY, NSEMS, IPC_CREAT | 0666);
    if(sem_id < 0) printf("Error in creating the semaphore./n");
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
    total = incrementByOne(total, valTo);
    printf("From Process %c: counter = %d\n", processNum, total);
    return total;
}

void initialize_shared_memory() {
    shmadd = (char *) 0;

    if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((total = (shared_mem *) shmat(shmid, NULL, 0)) == (shared_mem *) -1) {
        perror("shmat");
        exit(1);
    } // assist in creating shared mem
}