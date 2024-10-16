#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMKEY ((key_t) 1497) // macro to define shared memory

/**
 * Struct for shared memory, bound to variable total
 */
typedef struct {
    int value;
} shared_mem;
shared_mem *total;

/**
 * Increments input value (total) to a new declared value, child method to createProcessAfter
 * @param oldVal total global variable
 * @param newVal new value to assign to total
 * @return
 */
int incrementByOne(int oldVal, int newVal) {
    while (oldVal < newVal) oldVal++;
    return oldVal;
}//increment by 100k 200k 300k per each run


#include <sys/sem.h>

/* semaphore key */
#define SEMKEY ((key_t) 400L)
/* number of semaphores being created */
#define NSEMS 1

/* GLOBAL */

int sem_id; /* semaphore id */
/* semaphore buffers */
static struct sembuf OP = {0,-1,0};
static struct sembuf OV = {0,1,0};
struct sembuf *P = &OP;
struct sembuf *V = &OV;

/* semaphore union used to generate semaphore */
typedef union {
    int val;
    struct semid_ds *buf;
    ushort *array;
} semunion;

/* POP (wait()) function for semaphore to protect critical section */
int POP() {
    int status;
    status = semop(sem_id, P, 1);
    return status;
}

/* VOP (signal()) function for semaphore to release protection */
int VOP() {
    int status;
    status = semop(sem_id, V, 1);
    return status;
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

int main() {
    int shmid, pid1, pid2, pid3, pid4;
    char *shmadd;
    shmadd = (char *) 0;

    int value, value1;
    semunion semctl_arg;
    semctl_arg.val = 1;

    // Create semaphore set
    sem_id = semget(SEMKEY, NSEMS, IPC_CREAT | 0666);
    if (sem_id < 0) {
        perror("Error in creating semaphore");
        return 1;
    }

    // Initialize semaphore (SETVAL)
    semctl_arg.val = 1; // set to 1 to ride to rohan
    if (semctl(sem_id, 0, SETVAL, semctl_arg) == -1) {
        perror("Error in initializing semaphore");
        return 1;
    }

    if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((total = (shared_mem *) shmat(shmid, NULL, 0)) == (shared_mem *) -1) {
        perror("shmat");
        exit(1);
    } // assist in creating shared mem

    total->value = 0;

    (pid1 = fork()) == 0 ? (createProcessAfter(total->value, 100000, '1'), exit(0)) : 0;
    (pid2 = fork()) == 0 ? (createProcessAfter(total->value, 200000, '2'), exit(0)) : 0;
    (pid3 = fork()) == 0 ? (createProcessAfter(total->value, 300000, '3'), exit(0)) : 0;
    (pid4 = fork()) == 0 ? (createProcessAfter(total->value, 500000, '4'), exit(0)) : 0;
    //create all child processes


    for (int i = 0; i < 4; i++) {
        int status;
        pid_t child_pid = wait(&status);
        printf("Child with ID: %d has just exited.\n", child_pid);
    }//wait for children to exit after kill
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }
    /* De-allocate semaphore */
    semctl_arg.val = 0;
    int status = semctl(sem_id, 0, IPC_RMID, semctl_arg);
    if(status < 0) printf("Error in removing the semaphore.\n");

    shmctl(shmid, IPC_RMID, NULL);//detach and delete shared memory

    printf("End of Simulation.");

    return 0;
}
