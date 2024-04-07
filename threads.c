#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 10

typedef struct {
    int row;
    int col;
    char** matrix;
} ThreadData;

void* moveThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int row = data->row;
    int col = data->col;
    char** matrix = data->matrix;

    // Check if the next position is empty
    if (matrix[row + 1][col] == ' ') {
        row++;
    }

    // Print the current position
    printf("Thread moved to position [%d][%d]\n", row, col);

    // Clean up and exit the thread
    free(data);
    pthread_exit(NULL);
}

void createThread(char** matrix, int row, int col) {
    pthread_t thread;
    ThreadData* data = malloc(sizeof(ThreadData));
    data->row = row;
    data->col = col;
    data->matrix = matrix;

    // Create the thread
    pthread_create(&thread, NULL, moveThread, data);
    pthread_join(thread, NULL);
}

int main() {
    // Create a sample matrix
    char** matrix = malloc(MATRIX_SIZE * sizeof(char*));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = malloc(MATRIX_SIZE * sizeof(char));
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = ' ';
        }
    }

    // Set the starting position
    int startRow = 0;
    int startCol = 0;
    matrix[startRow][startCol] = 'S';

    // Create the thread
    createThread(matrix, startRow, startCol);

    // Free the memory
    for (int i = 0; i < MATRIX_SIZE; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}