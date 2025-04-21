/**
 * @file test_framework.c
 * @author Andrei Girjoaba (a.v.girjoaba@rug.nl)
 * @brief The implementation of a simple unit test C framework I used for working with
 * matrices. Contains helper functions and MACROS.
 * @version 0.1
 * @date 2024-05-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "test_framework.h"

/**
 * @brief Read the values of N and K from a file.
 * 
 * @param file FILE pointer to the file.
 * @param N value passed by reference.
 * @param K value passed by reference.
 */
void read_N_K(FILE *file, int *N, int *K) {
    int result = fscanf(file, "%d %d", N, K);
    if (result != 2) {
        printf("Read operation went wrong!\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Reads the number representation of a matrix from a file.
 * 
 * @param file FILE pointer to the file.
 * @param N used to allocate memory equal to the number of columns.
 * @return uint8_t* the allocated matrix.
 */
uint8_t *read_number_matrix(FILE *file, int N) {
    uint8_t *matrix = malloc(N*sizeof(uint8_t));
    int result;
    for (int i = 0; i < N; i++) {
        result = fscanf(file, "%hhd", matrix + i);
        if (result != 1) {
            printf("Read operation went wrong!\n");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

/**
 * @brief Helper function to convert the normal bit representation of a matrix into
 * the more compact number representation that we use. We also tranpose the matrix
 * to work directly with the columns.
 * 
 * @param file_path path to the file containing the matrix.
 */
uint8_t *read_bit_matrix(FILE *file, int N, int K) {
    int rows = N - K;
    uint8_t *matrix = malloc(rows*N*sizeof(uint8_t));
    int result;
    for (int i = 0; i < rows * N; i++) {
        result = fscanf(file, "%hhd", matrix + i);
        if (result != 1) {
            printf("Read operation went wrong!\n");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

void matrix2numbers(const char *file_path) {
    // Open File
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Could not open the file!\n");
        exit(EXIT_FAILURE);
    }

    // Read from File
    int N, K;
    read_N_K(file, &N, &K);
    uint8_t *matrix = read_bit_matrix(file, N, K);
    int i;
    int rows = N - K;
    int col_number;
    // for (int j = 0; j < N; j++) {
    //     i = 0;
    //     for (int i = rows - 1; i >= 0; i--) {
    //         printf("%d ", *(matrix + j*rows + i));   
    //     }
    //     printf("\n");
    // }
    // Find the number associated with each column
    for (int j = 0; j < N; j++) {
        i = 0;
        col_number = 0;
        for (int i = rows - 1; i >= 0; i--) {
            col_number <<= 1;
            col_number += *(matrix + j*rows + i);
        }
        if (col_number == 0) {
            printf("\n%d %d\n", i, j);
        }
        printf("%d ", col_number);
    }
    printf("\n");

    fclose(file);
}

uint8_t *matrix2transpose(const char *file_path) {
    // Open File
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Could not open the file!\n");
        exit(EXIT_FAILURE);
    }

    // Read from File
    int N, K;
    read_N_K(file, &N, &K);
    N = N - K;
    K *= -1;
    uint8_t *matrix = read_bit_matrix(file, N, K);

    // Find the number associated with each column
    int rows = N - K;
    // int col_number;
    int row_weight;
    for (int j = 0; j < N; j++) {
        row_weight = 0;
        for (int i = 0; i < rows; i++) {
            if (matrix[i * N + j] == 1) row_weight++;
            printf("%d", matrix[i * N + j]);
        }
        printf("\nRow %d weight: %d\n", j + 1, row_weight);
        printf("\n");
    }
    printf("\n");

    fclose(file);

    return matrix;
}