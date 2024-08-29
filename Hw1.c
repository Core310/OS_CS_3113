#include "Hw1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* arr1;
int* arr2;
int usrinp;

int main(void) {
    array_and_pointer();
    pointer();
    address_Space();
    loop_and_array();
    printf( "%d", declare_func());
    calc_condition_fw();
    fr();
}

void array_and_pointer() {
    usrinp =5;
    //int usrinp = scanf("%d");
    arr1 = malloc(usrinp * sizeof(int));
    arr2 = malloc(usrinp * sizeof(int));
}

void pointer() {
    printf("%s","\nSize of a: ");
    printf("%p",&arr1);
}

void address_Space() {
    printf("%s","\nSize of a: ");
    printf("%llu",sizeof(arr1));
}

void loop_and_array() {
    for(int i =0; i< sizeof(arr1); i++) {
        arr1[i] = usrinp;
        arr2[i] = usrinp;
    }
}

int declare_func(int o,int p) {
    return o*p;
}

void calc_condition_fw() {
    for (int i = 0; i < sizeof(arr1); ++i) {
        int rslt = arr1[i] * arr2[i];
        bool even_odd = !(rslt % 2);
        printf("%d ",rslt);
        printf("%d ",even_odd);


    }
}

void fr(){}