#include "Hw1.h"
#include <stdio.h>
#include <stdlib.h>

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
    free(arr1);
    free(arr2);
    return 0;
}

void array_and_pointer() {
    printf("Input array size");
    scanf("%d",&usrinp);
    arr1 = malloc(usrinp * sizeof(int));
    arr2 = malloc(usrinp * sizeof(int));
}

void pointer() {
    printf("%s","Adress of arrl after allocation: ");
    printf("%p",&arr1);
}

void address_Space() {
    printf("%s","\nSize of pointer arrl: ");
    printf("%llu", sizeof(arr1));
}

void loop_and_array() {
    printf("%s","\nEnter contents of arr1:\n");
    for(int i =0; i< usrinp; i++) {
        printf("Enter value of arr1[%d]: ",i);
        scanf("%d",&arr1[i]);
    }
    printf("%s","Enter contents of arr2:\n");
    for (int i = 0; i < usrinp; ++i) {
        printf("Enter value of arr2[%d]: ",i);
        scanf("%d",&arr2[i]);
    }
}

int declare_func(int o,int p) {
    return o*p;
}

void calc_condition_fw() {
    printf("%s", "Multiplication start.\nMultiplication done.");
    fptr = fopen("hw1_output.txt", "w");
    for (int i = 0; i < usrinp; i++) {
        int rslt = arr1[i] * arr2[i];
        fprintf(fptr, "\narr[%d] * arr2[%d] = [%d]\n", i, i, rslt);
        fprintf(fptr,"\n\t [%d] is an [%s] number.\n", rslt,even_odd(arr1[i],arr2[i]));
    }
    fclose(fptr);
}

const char* even_odd(int a, int b) {
    return (a * b) % 2 == 0 ? "even" : "odd";
}

void fr() {
    printf("%s","\nRead file.");
    fptr = fopen("hw1_output.txt", "r");
    char curCh;
    while ((curCh = fgetc(fptr)) != EOF) {
        printf("%c", curCh);
    }
    fclose(fptr);
}
