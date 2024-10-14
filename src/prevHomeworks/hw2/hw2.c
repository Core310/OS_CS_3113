//
// Created by arika on 4/10/2024.
//

#include "hw2.h"

#include <pthread.h>
#include <stdio.h>
/**
 * global variable acting as main counter for countUpMethod (run by each thread)
 */
int total =-1;

/**
 * /**
 * Main function to create and run threads. Calls make_thread() 10 times and prints total value after each call
 */
int main () {
    for (int i = 0; i < 10; i++)
        printf("Current value of total: %d\n" , make_thread());

    printf("\nFinal value of total: %d" , total);
    return 0;
}

/**
 * Function that creates a thread to run countUpTo method
 * @return total value after running countUpTo method (by 1000)
 */
int make_thread() {
    //make a new thread (called 10 times), and call countUpTo
    pthread_t new_thread;
    pthread_create(&new_thread, NULL, countUpTo, NULL);
    total = countUpTo(total,1000);
    return total;
}
/**
 * Function that increments a value by a given number
 * @param prev previous value
 * @param next number to increment by
 * @return incremented value
 */
int countUpTo(int prev,int next) {
    for (int i = 0; i < next; ++i) {
        prev++;
    }
    return prev;
}