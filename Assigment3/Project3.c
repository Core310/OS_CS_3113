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

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 15   // Circular buffer size as stated in the project document "15 positions"
#define FILENAME "mytest.dat" // Input file
#define FILE_ENDCHARCTER '*'
#define UNLOCK_MUTEX pthread_mutex_unlock(&buffer_mutex);
#define LOCK_MUTEX pthread_mutex_lock(&buffer_mutex);

// circ buffer vars
char buffer[BUFFER_SIZE];
int producerIdx = 0, consumerIdx = 0;

// Semaphores/mutex init
sem_t buffer_e_empty_slots;
sem_t buf_full_slots;
pthread_mutex_t buffer_mutex;


void readFile(FILE *fp) {
    char ch;
    while (fscanf(fp, "%c", &ch) != EOF) {//main file reader
        sem_wait(&buffer_e_empty_slots); //wait for slot
        LOCK_MUTEX // Lock buffer so only this process can use

        // add ch 2 buf
        buffer[producerIdx] = ch;
        producerIdx = (producerIdx + 1) % BUFFER_SIZE;
        //release slot + unlock buffer
        UNLOCK_MUTEX;
        sem_post(&buf_full_slots);
    }
}

void* producer(void* arg) {
    FILE* fp = fopen(FILENAME, "r"); // r for write
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    readFile(fp);
    //Tel is consumer is EOF with special char *
    sem_wait(&buffer_e_empty_slots);
    LOCK_MUTEX

    buffer[producerIdx] = FILE_ENDCHARCTER;
    producerIdx = (producerIdx + 1) % BUFFER_SIZE;

    UNLOCK_MUTEX;
    sem_post(&buf_full_slots);

    fclose(fp);
    return NULL;
}

void exit_condition(char ch) {
    if (ch != FILE_ENDCHARCTER) { // ! special char?
        printf("%c", ch);
        fflush(stdout);
    }
}

// Consumer thread function
void* consumer(void* arg) {
    char ch;//ch char ftw! No need any other names for the legendary ch char
    do { //wow first time I'm using a do while
        //Do same buffer stuff as producer
        sem_wait(&buf_full_slots);
        LOCK_MUTEX

        // more same stuff (maybe I could put it in a function)
        ch = buffer[consumerIdx];
        consumerIdx = (consumerIdx + 1) % BUFFER_SIZE;

        UNLOCK_MUTEX;
        sem_post(&buffer_e_empty_slots);

        exit_condition(ch);

        sleep(1); // sleep to make the consumer run slower as directed in p.2 of the document "The consumer should run slower than producer."
    } while (ch != FILE_ENDCHARCTER);

    return NULL;//oof no nullptr :(
}

int main() {
    // init semaphore + mutex
    sem_init(&buffer_e_empty_slots, 0, BUFFER_SIZE); // Start with all slots empty
    sem_init(&buf_full_slots, 0, 0);            // Start with no slots filled
    pthread_mutex_init(&buffer_mutex, NULL);

    // init producer/consumer thread
    pthread_t producer_tid, consumer_tid;
    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);

    // hold 4 threads 2 finish
    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    //-rm mutex + sems
    sem_destroy(&buffer_e_empty_slots);
    sem_destroy(&buf_full_slots);
    pthread_mutex_destroy(&buffer_mutex);

    return 0;
}