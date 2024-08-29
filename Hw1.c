#include "Hw1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* arr1;
int* arr2;
int usrinp;
FILE *fptr;

int main(void) {
    array_and_pointer();
    pointer();
    address_Space();
    loop_and_array();
    calc_condition_fw();
    fr();
}

void array_and_pointer() {
    printf("Input array size");
    int usrinp = scanf("%d");
    arr1 = malloc(usrinp * sizeof(int));
    arr2 = malloc(usrinp * sizeof(int));
}

void pointer() {
    printf("%s","\nAdress of arrl after allocation: ");
    printf("%p",&arr1);
}

void address_Space() {
    printf("%s","\nSize of pointer arrl: ");
    printf("%llu",sizeof(arr1));
}

void loop_and_array() {
    printf("%s","Enter contents of arr1:");
    for(int i =0; i< sizeof(arr1); i++) {
        printf("Enter value of arr1[%d]",i);
        arr1[i] = scanf("%d",&arr1);
    }
    printf("%s","Enter contents of arr2:");
    for (int i = 0; i < sizeof(arr2); ++i) {
        printf("Enter value of arr2[%d]",i);
        arr1[i] = scanf("%d",&arr2);
    }
}

int declare_func(int o,int p) {
    return o*p;
}

void calc_condition_fw() {
    printf("%s", "Multiplication start.\nMultiplication done.");
    fptr = fopen("hw1_output.txt", "w");
    for (int i = 0; i < sizeof(arr1); ++i) {
        int rslt = arr1[i] * arr2[i];
        fprintf(fptr, "[%d]\n",rslt);
        fprintf(fptr,"\t [%d] is an [%s] number.\n", rslt,even_odd(arr1[i],arr2[i]));
    }
    fclose(fptr);
}

const char* even_odd(int a, int b) {
    return (a * b) % 2 == 0 ? "even" : "odd";
}

void fr() {
    printf("%s","Read file.");
    fptr = fopen("hw1_output.txt", "r");
    while (const char* c = nullptr) {
        printf("[%c]\n",c);
    }
    fclose(fptr);
}
