#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "diccionario.c"
#define ARRIBA 0
#define ABAJO 1
#define IZQUIERDA 2
#define DERECHA 3

#define ESQUINA_SUPERIOR_IZQUIERDA 0
#define ESQUINA_SUPERIOR_DERECHA 1
#define ESQUINA_INFERIOR_IZQUIERDA 2
#define ESQUINA_INFERIOR_DERECHA 3
#define LADO_DERECHO 4
#define LADO_IZQUIERDO 5
#define LADO_SUPERIOR 6
#define LADO_INFERIOR 7
#define CENTRO 8

#define MURO 0
#define META 1
#define PASILLO 2

struct Casilla {
    struct diccionario* direccionesCaminadas;
    int posicionMatriz;
    int tipo;
    int fila;
    int columna;
    char elemento;
};

struct GestorLaberinto {
    struct Casilla** laberinto;
    int filaMax;
    int columnaMax;
    int intervalo_segundos;
    struct hiloArgumentos* hilos;
    int contadorHilos;
    int direccion;
};
struct hiloArgumentos{
    int direccion;
    int filaActual;
    int columnaActual;
    bool terminado;
    int numeroHilo;

};
/*void* gestionaHilos(void* arg) {
    struct GestorLaberinto* gestor = (struct GestorLaberinto*)arg;
    struct hiloArgumentos* hilos = malloc(gestor->filaMax*gestor->columnaMax * sizeof(struct hiloArgumentos));
    gestor->hilos=hilos;

    pthread_t hiloInicial;
    pthread_create(&hiloInicial, NULL, moverHilo, (void*)gestor);
    pthread_join(hiloInicial, NULL);
}
void* moverHilo(void* arg) {
    struct GestorLaberinto* gestor = (struct GestorLaberinto*)arg;

}*/
void* imprimir_laberinto_periodicamente(void* arg) {
    struct GestorLaberinto* gestor = (struct GestorLaberinto*)arg;

    while (1) {
        printf("Laberinto actual:\n");
        for (int i = 0; i < gestor->filaMax; i++) {
            for (int j = 0; j < gestor->columnaMax; j++) {
                printf("%c", gestor->laberinto[i][j].elemento);
            }
            printf("\n");
        }
        printf("\n");

        sleep(gestor->intervalo_segundos);
    }

    return NULL;
}

struct GestorLaberinto* leer_laberinto(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    struct GestorLaberinto* gestor = malloc(sizeof(struct GestorLaberinto));
    if (gestor == NULL) {
        printf("Error al asignar memoria.\n");
        exit(EXIT_FAILURE);
    }

    int fila, columna;
    fscanf(archivo, "%d %d", &fila, &columna);
    gestor->filaMax = fila;
    gestor->columnaMax = columna;
    gestor->direccion = DERECHA;

    fgetc(archivo);

    struct Casilla** laberinto = malloc(fila * sizeof(struct Casilla*));
    if (laberinto == NULL) {
        printf("Error al asignar memoria.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fila; i++) {
        laberinto[i] = malloc(columna * sizeof(struct Casilla));
        if (laberinto[i] == NULL) {
            printf("Error al asignar memoria.\n");
            exit(EXIT_FAILURE);
        }
    }

    char letra;
    for (int i = 0; i < fila; i++) {
        for (int j = 0; j < columna; j++) {
            letra = fgetc(archivo);
            laberinto[i][j].elemento = letra;
            laberinto[i][j].fila = i;
            laberinto[i][j].columna = j;
            laberinto[i][j].direccionesCaminadas=malloc(sizeof(struct diccionario));
            insertar(laberinto[i][j].direccionesCaminadas, ARRIBA, false);
            insertar(laberinto[i][j].direccionesCaminadas, ABAJO, false);
            insertar(laberinto[i][j].direccionesCaminadas, IZQUIERDA, false);
            insertar(laberinto[i][j].direccionesCaminadas, DERECHA, false);
            if (i == 0 && j == 0)
                laberinto[i][j].posicionMatriz = ESQUINA_SUPERIOR_IZQUIERDA;
            else if (i == 0 && j == columna - 1)
                laberinto[i][j].posicionMatriz = ESQUINA_SUPERIOR_DERECHA;
            else if (i == fila - 1 && j == 0)
                laberinto[i][j].posicionMatriz = ESQUINA_INFERIOR_IZQUIERDA;
            else if (i == fila - 1 && j == columna - 1)
                laberinto[i][j].posicionMatriz = ESQUINA_INFERIOR_DERECHA;
            else if (i == 0)
                laberinto[i][j].posicionMatriz = LADO_SUPERIOR;
            else if (i == fila - 1)
                laberinto[i][j].posicionMatriz = LADO_INFERIOR;
            else if (j == 0)
                laberinto[i][j].posicionMatriz = LADO_IZQUIERDO;
            else if (j == columna - 1)
                laberinto[i][j].posicionMatriz = LADO_DERECHO;
            else
                laberinto[i][j].posicionMatriz = CENTRO;

            if (letra == '*')
                laberinto[i][j].tipo = MURO;
            if (letra == ' ')
                laberinto[i][j].tipo = PASILLO;
            if (letra == '/')
                laberinto[i][j].tipo = META;
        }
        fgetc(archivo);
    }

    gestor->laberinto = laberinto;
    fclose(archivo);
    return gestor;
}

bool revisar_adjacentes_t(int row, int column, struct GestorLaberinto* gestorLaberinto) {
    int filaMax = gestorLaberinto->filaMax;
    struct Casilla** laberinto = gestorLaberinto->laberinto;
    // Check if the top space is within bounds
    if (row - 1 < 0) {
        return false;
    }
    
    return true;
}

bool revisar_adjacentes_b(int row, int column, struct GestorLaberinto* gestorLaberinto) {
    int filaMax = gestorLaberinto->filaMax;
    struct Casilla** laberinto = gestorLaberinto->laberinto;
    // Check if the bottom space is within bounds
    if (row + 1 >= filaMax) {
        return false;
    }
    
    return true;
}

bool revisar_adjacentes_i(int row, int column, struct GestorLaberinto* gestorLaberinto) {
    int columnaMax = gestorLaberinto->columnaMax;
    struct Casilla** laberinto = gestorLaberinto->laberinto;
    // Check if the left space is within bounds
    if (column - 1 < 0) {
        return false;
    }
    
    return true;
}

bool revisar_adjacentes_d(int row, int column, struct GestorLaberinto* gestorLaberinto) {
    int columnaMax = gestorLaberinto->columnaMax;
    struct Casilla** laberinto = gestorLaberinto->laberinto;
    // Check if the right space is within bounds
    if (column + 1 >= columnaMax) {
        return false;
    }
    
    return true;
}

//Creador de hilos
void* creador(void* arg) {
    struct GestorLaberinto* gestor = (struct GestorLaberinto*)arg;
    struct Casilla** laberinto = (struct Casilla**) gestor->laberinto;
    int filaActual = 0;
    int columnaActual = 0;
    int direccion = gestor->direccion;
    bool terminado = false;
    int numeroHilo = 0;

    while (!terminado) {
        struct Casilla* casillaActual = &laberinto[filaActual][columnaActual];
        // Check if the current position is a wall or has already been visited 
        if (casillaActual->tipo == MURO) {
            // Change the direction to the next one
            printf("I've hit a wall at: %d %d\n", filaActual, columnaActual);
            terminado = true;
        }else if (casillaActual->tipo == META) {
            // The thread has reached the end of the maze
            printf("I've reached the end\n");
            terminado = true;
        }
        
        // Check the spaces on the opposite axis to see if their "tipo" is pasillo
        if (direccion == ARRIBA || direccion == ABAJO) {
            if(revisar_adjacentes_i(filaActual, columnaActual, gestor)){
                struct Casilla* casillaIzquierda = &laberinto[filaActual][columnaActual - 1];
                if (casillaIzquierda->tipo == PASILLO) {
                    // Do something
                    printf("Espacio en casilla izquierda de: %d %d\n", filaActual, columnaActual);
                }
            }
            if(revisar_adjacentes_d(filaActual, columnaActual, gestor)){
                struct Casilla* casillaDerecha = &laberinto[filaActual][columnaActual + 1];
                if (casillaDerecha->tipo == PASILLO) {
                    // Do something
                    printf("Espacio en casilla derecha de: %d %d\n", filaActual, columnaActual);
                }
            }
        } else if (direccion == IZQUIERDA || direccion == DERECHA) {
            if (revisar_adjacentes_t(filaActual, columnaActual, gestor)) {
                struct Casilla* casillaArriba = &laberinto[filaActual - 1][columnaActual];
                if (casillaArriba->tipo == PASILLO) {
                    // Do something
                    printf("Espacio en casilla arriba de: %d %d\n", filaActual, columnaActual);
                }
            }
            if(revisar_adjacentes_b(filaActual, columnaActual, gestor)){
                struct Casilla* casillaAbajo = &laberinto[filaActual + 1][columnaActual];
                if (casillaAbajo->tipo == PASILLO) {
                    // Do something
                    printf("Espacio en casilla abajo de: %d %d\n", filaActual, columnaActual);
                }
            }
        }

        // Move to the next position based on the current direction
        switch (direccion) {
            case ARRIBA:
                filaActual--;
                break;
            case ABAJO:
                filaActual++;
                break;
            case IZQUIERDA:
                columnaActual--;
                break;
            case DERECHA:
                columnaActual++;
                break;
        } 
        //Check if the new position is within the bounds of the maze
        if (filaActual < 0 || filaActual >= gestor->filaMax || columnaActual < 0 || columnaActual >= gestor->columnaMax) {
            // The thread has reached the end of the maze
            terminado = true;
        }
    }

    return NULL;
}

int main() {
    const char* nombre_archivo = "laberinto.txt";

    struct GestorLaberinto* gestorLaberinto;


    gestorLaberinto = leer_laberinto(nombre_archivo);

    printf("Dimensiones del laberinto:\n");
    printf("Filas: %d\n", gestorLaberinto->filaMax);
    printf("Columnas: %d\n", gestorLaberinto->columnaMax);

    gestorLaberinto->intervalo_segundos = 5;

    pthread_t thread_impresion;
    pthread_create(&thread_impresion, NULL, imprimir_laberinto_periodicamente, (void*)gestorLaberinto);

    pthread_t thread_creador;
    pthread_create(&thread_creador, NULL, creador, (void*)gestorLaberinto);

    pthread_join(thread_impresion, NULL);
    pthread_join(thread_creador, NULL);

    return 0;
}
