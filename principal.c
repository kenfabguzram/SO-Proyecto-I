#include <stdio.h>
#include <stdlib.h>

// Función para leer el laberinto desde un archivo y almacenarlo en una matriz
char** leer_laberinto(const char* nombre_archivo, int* filas, int* columnas) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    // Leer las dimensiones del laberinto desde la primera línea
    fscanf(archivo, "%d %d", filas, columnas);
    
    // Ignorar el salto de línea
    fgetc(archivo);

    // Crear la matriz para almacenar el laberinto
    char** laberinto = (char**)malloc((*filas) * sizeof(char*));
    if (laberinto == NULL) {
        printf("Error al asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *filas; i++) {
        laberinto[i] = (char*)malloc((*columnas) * sizeof(char));
        if (laberinto[i] == NULL) {
            printf("Error al asignar memoria.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Leer las líneas restantes del archivo y almacenarlas en la matriz
    for (int i = 0; i < *filas; i++) {
        for (int j = 0; j < *columnas; j++) {
            laberinto[i][j] = fgetc(archivo);
        }
        // Ignorar el salto de línea
        fgetc(archivo);
    }

    fclose(archivo);
    return laberinto;
}

// Función para imprimir el laberinto
void imprimir_laberinto(char** laberinto, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%c", laberinto[i][j]);
        }
        printf("\n");
    }
}

int main() {
    const char* nombre_archivo = "laberinto.txt";
    int filas, columnas;

    // Leer el laberinto desde el archivo
    char** laberinto = leer_laberinto(nombre_archivo, &filas, &columnas);

    // Imprimir el laberinto
    printf("Laberinto original:\n");
    imprimir_laberinto(laberinto, filas, columnas);
    printf("\n");

    // Imprimir las dimensiones del laberinto
    printf("Dimensiones del laberinto:\n");
    printf("Filas: %d\n", filas);
    printf("Columnas: %d\n", columnas);

    // Liberar memoria
    for (int i = 0; i < filas; i++) {
        free(laberinto[i]);
    }
    free(laberinto);

    return 0;
}