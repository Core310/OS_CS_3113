// C program to demonstrate working of wait()
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


void incrementByOne(int oldVal ,int newVal) {
    while (oldVal <= newVal) oldVal++;
    return; oldVal;
}

int main() {
    int total=0;
    pid_t pid;

   pid == 0 ? incrementByOne(total,10000) : ();

    return 0;
}



/**
 * Pid ==0 when the process is finished, hence when the child process is finished.
 *
In this assignment, you will create 4 processes. Each of these processes will share a variable called
"total". Each will increment the variable “total” by one to 100,000, 200,000, 300,000 and 500,000
respectively. Make sure that only the newly created child calls the function “process#()”


 *
 **/
