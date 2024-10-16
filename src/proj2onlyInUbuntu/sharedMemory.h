#pragma once

/**
 * Struct for shared memory, bound to variable total
 */
typedef struct {
    int value;
} shared_mem;
shared_mem *total;

void initialize_shared_memory();
int incrementByOne(int oldVal, int newVal);
int createProcessAfter(int total, int valTo, char processNum);
void initialize_shared_memory();
void waitForChildren();
void detach_and_cleanup_shared_memory()