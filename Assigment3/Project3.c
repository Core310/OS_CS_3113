//
// Created by arika on 7/11/2024.
//

#include "Project3.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 15   // Circular buffer size
#define FILENAME "mytest.dat" // Input file

// Circular buffer
char buffer[BUFFER_SIZE];
int in = 0;  // Producer index
int out = 0; // Consumer index

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
        buffer[in] = ch;
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&full_slots); // Signal a filled slot
    }

    // Indicate the end of the file with a special character '*'
    sem_wait(&empty_slots);
    pthread_mutex_lock(&mutex);
    buffer[in] = '*';
    in = (in + 1) % BUFFER_SIZE;
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
        ch = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&empty_slots); // Signal an empty slot

        if (ch != '*') {
            printf("%c", ch); // Print the character
            fflush(stdout);
        }

        sleep(1); // Consumer runs slower
    } while (ch != '*'); // Stop when the special character is read

    return NULL;
}

int main() {
    // Initialize semaphores and mutex
    sem_init(&empty_slots, 0, BUFFER_SIZE); // Start with all slots empty
    sem_init(&full_slots, 0, 0);            // Start with no slots filled
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_t producer_tid, consumer_tid;
    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&mutex);

    return 0;
}
