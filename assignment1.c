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
shared_mem *total;

int incrementByOne(int oldVal, int newVal) {
    while (oldVal < newVal) oldVal++;
    return oldVal;
}

int createProcessAfter(int total, int valTo, char processNum) {
    total = incrementByOne(total, valTo);
    printf("From Process %c: counter = %d\n", processNum, total);
    return total;
}

int main() {
    int shmid, pid1, pid2, pid3, pid4;
    char *shmadd;
    shmadd = (char *) 0;

    if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((total = (shared_mem *) shmat(shmid, NULL, 0)) == (shared_mem *) -1) {
        perror("shmat");
        exit(1);
    }

    total->value = 0;

    (pid1 = fork()) == 0 ? (createProcessAfter(total->value, 10000, '1'), exit(0)) : 0;
    (pid2 = fork()) == 0 ? (createProcessAfter(total->value, 20000, '2'), exit(0)) : 0;
    (pid3 = fork()) == 0 ? (createProcessAfter(total->value, 30000, '3'), exit(0)) : 0;
    (pid4 = fork()) == 0 ? (createProcessAfter(total->value, 40000, '4'), exit(0)) : 0;
    for (int i = 0; i < 4; i++) {
        int status;
        pid_t child_pid = wait(&status);
        printf("Child with ID: %d has just exited.\n", child_pid);
    }

    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }
    shmctl(shmid, IPC_RMID, NULL);

    printf("End of Simulation.");

    return 0;
}