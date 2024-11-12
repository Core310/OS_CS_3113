//
// Created by arika on 7/11/2024.
//

/*
*The purpose is to learn how to use semaphores to protect a limited size resource. A circular
buffer with 15 positions (each position stores 1 character) is to be used to communicate
information between two threads (producer and consumer). The producer thread will read
characters, one by one from a file and place it in the buffer and continue to do that until the “end
of-file” (EOF) marker is reached. The name of the file must be “mytest.dat” when you are
submitting the program – of course you can use your own file while individually testing your
program.  There should be no more than 150 characters in the file. The producer must inform the
consumer when it has finished placing the last character in the buffer. The producer could do this
by placing a special character for example, “*” in the shared buffer or by using a shared memory
flag that the producer sets to be true and the consumer reads at the appropriate time.
Consumer thread will read the characters, one by one, from the shared buffer and print it to the
screen. A parent process will create both producer and consumer threads and will wait until both
are finished to destroy semaphores. The consumer should run slower than producer. So, place a
one second sleep in the consumer thread between “reads” from the shared memory.
 */

#include "Project3.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 15   // Circular buffer size as stated in the project document "15 positions"
#define FILENAME "mytest.dat" // Input file

// Circular buffer
char buffer[BUFFER_SIZE];
int producerIdx = 0;
int consumerIdx = 0;

// Semaphores and mutex
sem_t empty_slots;  // To track empty slots in the buffer
sem_t full_slots;   // To track filled slots in the buffer
pthread_mutex_t mutex; // Mutex for buffer access

// Producer thread function
void* producer(void* arg) {
    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char ch;
    while (fscanf(fp, "%c", &ch) != EOF) {
        sem_wait(&empty_slots); // Wait for an empty slot
        pthread_mutex_lock(&mutex); // Lock the buffer

        // Add the character to the buffer
        buffer[producerIdx] = ch;
        producerIdx = (producerIdx + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&full_slots); // Signal a filled slot
    }

    // Indicate the end of the file with a special character '*'
    sem_wait(&empty_slots);
    pthread_mutex_lock(&mutex);
    buffer[producerIdx] = '*';// char to end producer sequence
    producerIdx = (producerIdx + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&full_slots);

    fclose(fp);
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    char ch;
    do {
        sem_wait(&full_slots); // Wait for a filled slot
        pthread_mutex_lock(&mutex); // Lock the buffer

        // Read the character from the buffer
        ch = buffer[consumerIdx];
        consumerIdx = (consumerIdx + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&empty_slots); // Signal an empty slot

        if (ch != '*') { // exit condition
            printf("%c", ch); // Print the character
            fflush(stdout);
        }

        sleep(1); // sleep to make the consumer run slower as directed in p.2 of the document "The consumer should run slower than producer."
    } while (ch != '*'); // Stop when the special character is read

    return NULL;
}

int main() {
    // init semaphore + mutex
    sem_init(&empty_slots, 0, BUFFER_SIZE); // Start with all slots empty
    sem_init(&full_slots, 0, 0);            // Start with no slots filled
    pthread_mutex_init(&mutex, NULL);

    // init producer/consumer thread
    pthread_t producer_tid, consumer_tid;
    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);

    // hold 4 threads 2 finish
    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    //-rm mutex + sems
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&mutex);

    return 0;
}