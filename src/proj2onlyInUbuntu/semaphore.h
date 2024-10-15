#pragma once
#include <sys/sem.h>

/* semaphore key */
#define SEMKEY ((key_t) 400L)
/* number of semaphores being created */
#define NSEMS 1

/* GLOBAL */

int sem_id; /* semaphore id */
/* semaphore buffers */
static struct sembuf OP = {0,-1,0};
static struct sembuf OV = {0,1,0};
struct sembuf *P=&OP;
struct sembuf *V=&OV;

/**
 * semaphore union used to generate semaphore
 */
typedef union {
    int val;
    struct semid_ds *buf;
    ushort *array;
} semunion;

/**
 * POP (wait()) function for semaphore to protect critical section
 * @return status (semop(sem_id, P, 1);
 */
inline int POP()
{
    int status;
    status = semop(sem_id, P, 1);
    return status;
}

/**
 * VOP (signal()) function for semaphore to release protection
 * @return status (semop(sem_id, V, 1);
 */
inline int VOP()
{
    int status;
    status = semop(sem_id, V, 1);
    return status;
}
