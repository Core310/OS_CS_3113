// C program to demonstrate working of wait()
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int incrementByOne(int oldVal ,int newVal) {
    while (oldVal <= newVal) oldVal++;
    return oldVal;
}

int createProcessAfter(int total, int processNumber, int valTo) {
    total = incrementByOne(total,valTo), printf("From Process %d: counter = %d\n",processNumber,total);
    return total;
}

int main() {
    int total=0;
    int p1=1,p2=1,p3=3,p4=4;
    pid_t pid;
    p1 = fork();
    p1 == 0 ? (total = createProcessAfter(total,1,100000), p2 = fork()): 0;
    p2 == 0 ? (total = createProcessAfter(total,2,200000), p3 = fork()): 0;
    p3 == 0 ? (total = createProcessAfter(total,3,300000), p4 = fork()): 0;
    p4 == 0 ? (createProcessAfter(total,4,500000)): 0;

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
