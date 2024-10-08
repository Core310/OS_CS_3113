//
// Created by arika on 4/10/2024.
//

#include "hw2.h"

#include <pthread.h>
#include <stdio.h>
int total =-1;
int main () {
    for (int i = 0; i < 10; i++) {
        make_thread();
        printf("Current value of total: %d\n" , total);
    }

    printf("\nFinal value of total: %d" , total);
    return 0;
}

int make_thread() {
    //make a new thread (called 10 times), and call countUpTo
    pthread_t new_thread;
    pthread_create(&new_thread, NULL, countUpTo, NULL);
    total = countUpTo(total,1000);
}

int countUpTo(int prev,int next) {
    for (int i = 0; i < next; ++i) {
        prev++;
    }
    return prev;
}