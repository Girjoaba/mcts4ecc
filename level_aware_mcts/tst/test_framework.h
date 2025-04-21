/**
 * @file test_framework.c
 * @author Andrei Girjoaba (a.v.girjoaba@rug.nl)
 * @brief The header file of a simple unit test C framework I used for working with
 * matrices. Contains helper functions and MACROS.
 * @version 0.1
 * @date 2024-05-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TESTFRAMEWORK_H
#define TESTFRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Macro to assert that two values are equal
#define ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            fprintf(stderr, "Assertion failed: %s == %s, file %s, line %d\n", #expected, #actual, __FILE__, __LINE__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

// Macro to run a test case and print its name
#define RUN_TEST(test) \
    do { \
        printf("Running %s...\n", #test); \
        test(); \
        printf("%s passed.\n", #test); \
    } while (0)

/**
 * @brief Read the values of N and K from a file.
 * 
 * @param file FILE pointer to the file.
 * @param N value passed by reference.
 * @param K value passed by reference.
 */
void read_N_K(FILE *file, int *N, int *K);

/**
 * @brief Reads the number representation of a matrix from a file.
 * 
 * @param file FILE pointer to the file.
 * @param N used to allocate memory equal to the number of columns.
 * @return uint8_t* the allocated matrix.
 */
uint8_t *read_number_matrix(FILE *file, int N);

/**
 * @brief Helper function to convert the normal bit representation of a matrix into
 * the more compact number representation that we use. We also tranpose the matrix
 * to work directly with the columns.
 * 
 * @param file_path path to the file containing the matrix.
 */
void matrix2numbers(const char *file_path);

uint8_t *matrix2transpose(const char *file_path);

#endif