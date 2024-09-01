//
// Created by arika on 8/29/2024.
#pragma once

/**
* Declare two integer arrays with the same size using pointers and
dynamic memory allocation. The size should be determined by a user’s input.
 */
void array_and_pointer();
/**
 * Print the address of the first array.
 */
void pointer();
/**
* Try the operator sizeof and print size of the first array pointer. Compare
with the above one and think about the reason.
 */
void address_Space();
/**
*  Use the array size provided by the user and assign an integer value to
the array’s element. Again, each integer value must be provided by the user. Repeat the
procedure to fill both arrays we declared.
 */
void loop_and_array();
/**
 *Declare a function with two integer parameters, which calculates
and returns the multiple of the two numbers.
 */
int declare_func(int o,int p);
/**
* Using the function we declared,
multiply i-th element of the first array by i-th element of the second array. For each
result, check if the multiplication result is an even or odd number using condition
statements. Declare a file pointer and write the multiplication results in the new file
named “hw1_output.txt”. Once the file write is done, do not forget to close the file.
 */
void calc_condition_fw();
/**
*Open “hw1_output.txt” file in your program, read, and print the content of
the file. Again, please close the file.\
 */
void fr();

/**
 * smaller helper method for calccondition
 * @param a
 * @param b
 * @return
 */
const char* even_odd(int a, int b);
