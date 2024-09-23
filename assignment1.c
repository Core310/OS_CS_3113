#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMKEY ((key_t) 1497)

typedef struct {
    int value;
} shared_mem;

int incrementByOne(int oldVal, int newVal) {
    while (oldVal < newVal) oldVal++;
    return oldVal;
}

int createProcessAfter(int total, int processNumber, int valTo) {
    total = incrementByOne(total, valTo);
    printf("From Process %d: counter = %d\n", processNumber, total);
    return total;
}

void waitForPID() {
    wait(getpid());
    printf("Child with ID: %d has just exited.\n", getpid());
}

void makeChildProcess(shared_mem *total, int *p, int valTo) {
    total->value = createProcessAfter(total->value, 1, valTo);
    *p = fork();
    waitForPID();
}

int main() {
    int shmid;
    shared_mem *total;

    if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((total = (shared_mem *) shmat(shmid, NULL, 0)) == (shared_mem *) -1) {
        perror("shmat");
        exit(1);
    }

    total->value = 0;
    int p1, p2 = 1, p3 = 3, p4 = 4;
    p1 = fork();
    p1 == 0 ? makeChildProcess(total, &p2, 100000) : 0;
    p2 == 0 ? makeChildProcess(total, &p3, 200000) : 0;
    p3 == 0 ? makeChildProcess(total, &p4, 300000) : 0;
    p4 == 0 ? (createProcessAfter(total->value, 4, 500000), waitForPID()) : 0;

    if (shmdt(total) == -1) {
        perror ("shmdt");
        exit (-1);
    }
    shmctl(shmid, IPC_RMID,NULL);

    return 0;
}