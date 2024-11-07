
#pragma once

#define SHMKEY ((key_t) 1497) // macro to define shared memory

/**
* Struct for shared memory, bound to variable total
 */
typedef struct {
    int value;
} shared_mem;
shared_mem *total;
