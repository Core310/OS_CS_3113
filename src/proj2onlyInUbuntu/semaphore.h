#pragma once


#define SEMKEY ((key_t) 1498) // macro to define semaphore key


/* Semaphore union used to initialize semaphore */
typedef union {
    int val;
    struct semid_ds *buf;
    ushort *array;
} semunion;

int sem_id;  // semaphore id

/* semaphore buffers for P (wait) and V (signal) operations */
static struct sembuf OP = {0, -1, 0};  // P operation (wait)
static struct sembuf OV = {0, 1, 0};   // V operation (signal)
struct sembuf *P = &OP;
struct sembuf *V = &OV;

/* Semaphore wait operation */
inline void semaphore_wait() {
    if (semop(sem_id, P, 1) == -1) {
        perror("semop P");
        exit(1);
    }
}

/* Semaphore signal operation */
inline void semaphore_end_signal() {
    if (semop(sem_id, V, 1) == -1) {
        perror("semop V");
        exit(1);
    }
}