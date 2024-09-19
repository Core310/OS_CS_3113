// C program to demonstrate working of wait()
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

/**
 *
 * @param oldVal total
 * @param newVal value to increment by
 * @return 
 */
int incrementByOne(int oldVal ,int newVal) {
    while (oldVal < newVal) oldVal++;
    return oldVal;
}

int createProcessAfter(int total, int processNumber, int valTo) {
    total = incrementByOne(total,valTo), printf("From Process %d: counter = %d\n",processNumber,total);
    return total;
}

void waitForPID() {
    wait(getpid()), printf("Child with ID: %d has just exited.\n",getpid());
}

void makeChildProcess(int *total, int *p,int valTo) {
    return (*total = createProcessAfter(*total, 1, valTo), *p = fork(), waitForPID());
}


int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    int *total = shmat(shmid, NULL, 0);
    if (total == (int *)-1) {
        perror("shmat");
        exit(1);
    }
    *total = 0;

    int p1, p2 = 1, p3 = 3, p4 = 4;
    p1 = fork();
    p1 == 0 ? makeChildProcess(total, &p2, 100000) : 0;
    p2 == 0 ? makeChildProcess(total, &p3, 200000) : 0;
    p3 == 0 ? makeChildProcess(total, &p4, 300000) : 0;
    p4 == 0 ? (createProcessAfter(*total, 4, 500000), waitForPID()) : 0;

    // Detach and remove the shared memory segment
    shmdt(total);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}


/**
 Pid ==0 when the process is finished, hence when the child process is finished.

In this assignment, you will create 4 processes. Each of these processes will share a variable called
"total". Each will increment the variable “total” by one to 100,000, 200,000, 300,000 and 500,000
respectively. Make sure that only the newly created child calls the function “process#()”

From Process 1: counter = 270547.
From Process 2: counter = 347860.
From Process 3: counter = 400001.
From Process 4: counter = 500000.
Child with ID: 2412 has just exited.
Child with ID: 2411 has just exited.
Child with ID: 2413 has just exited.
Child with ID: 2415 has just exited
 *
 **/
