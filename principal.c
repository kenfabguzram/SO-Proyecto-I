#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "diccionario.c" // Este archivo debe contener la implementación de las funciones del diccionario

#define ARRIBA '^'
#define ABAJO 'v'
#define IZQUIERDA '<'
#define DERECHA '>'




// Definición de constantes para el tipo de casilla
#define MURO 0
#define META 1
#define PASILLO 2 // Definición de PASILLO

// Estructura para representar una casilla del laberinto
struct Casilla {
    struct diccionario* direccionesCaminadas;
    int posicionMatriz;
    int tipo;
    int fila;
    int columna;
    char elemento;
};

// Estructura para representar el gestor del laberinto
struct GestorLaberinto {
    pthread_mutex_t mutex;
    struct Casilla** laberinto;
    int filaMax;
    int columnaMax;
    int intervalo_segundos;
    int numeroHilos;
    pthread_t* hilos;
    struct HiloArgumentos* arg;
};

// Estructura para pasar argumentos a cada hilo
struct HiloArgumentos {
    struct GestorLaberinto* gestor;
    int direccion;
    int filaActual;
    int columnaActual;
    int idHilo;
};
void creaHilo(struct GestorLaberinto* gestor,int direccion,int filaActual, int columnaActual);

// Función para moverse en el laberinto
void moverse(struct HiloArgumentos* arg) {
    struct GestorLaberinto* gestor = arg->gestor;
    int fila = arg->filaActual;
    int columna = arg->columnaActual;
    int direccion = arg->direccion;
    int idHilo=arg->idHilo;
    int direccionOpuesta;
    switch(direccion){
        case ARRIBA:
            direccionOpuesta=ABAJO;
            break;
        case IZQUIERDA:
            direccionOpuesta=DERECHA;
            break;
        case ABAJO:
            direccionOpuesta=ARRIBA;
            break;
        case DERECHA:
            direccionOpuesta=IZQUIERDA;
            break;
    }

    while (fila >= 0 &&
     fila < gestor->filaMax &&
     columna >= 0 && 
     columna < gestor->columnaMax && 
     gestor->laberinto[fila][columna].tipo == PASILLO && 
     !buscar(gestor->laberinto[fila][columna].direccionesCaminadas, direccion) &&
     !buscar(gestor->laberinto[fila][columna].direccionesCaminadas, direccionOpuesta)
     ) {
        // Marcar la dirección como visitada
        insertar(gestor->laberinto[fila][columna].direccionesCaminadas, direccion, true);
        gestor->laberinto[fila][columna].elemento=(char)direccion;
        printf("Hilo %d en dirección %c está en fila %d, columna %d\n",idHilo,(char) direccion, fila, columna);
        sleep(gestor->intervalo_segundos);
        // Imprimir movimiento en pantalla
            // Determinar la nueva posición según la dirección
        switch (direccion) {
            case ARRIBA:
                if(columna-1 >= 0 &&
                columna-1 < gestor->columnaMax &&
                gestor->laberinto[fila][columna-1].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila][columna-1].direccionesCaminadas, IZQUIERDA)){
                    creaHilo(gestor,IZQUIERDA,fila, columna-1);
                }
                if(columna+1 >= 0 &&
                columna+1 < gestor->columnaMax &&
                gestor->laberinto[fila][columna+1].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila][columna+1].direccionesCaminadas, DERECHA)){
                    creaHilo(gestor,DERECHA,fila, columna+1);
                }
                
                fila--;
                break;
            case ABAJO:
                if(columna-1 >= 0 &&
                columna-1 < gestor->columnaMax &&
                gestor->laberinto[fila][columna-1].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila][columna-1].direccionesCaminadas, IZQUIERDA)){
                    creaHilo(gestor,IZQUIERDA,fila, columna-1);
                }
                if(columna+1 >= 0 &&
                columna+1 < gestor->columnaMax &&
                gestor->laberinto[fila][columna+1].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila][columna+1].direccionesCaminadas, DERECHA)){
                    creaHilo(gestor,DERECHA,fila, columna+1);
                }
                fila++;
                break;
            case IZQUIERDA:
                if(fila+1 >= 0 &&
                fila+1 < gestor->filaMax &&
                gestor->laberinto[fila+1][columna].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila+1][columna].direccionesCaminadas, ABAJO)){
                    creaHilo(gestor,ABAJO,fila+1, columna);
                }
                if(fila-1 >= 0 &&
                fila-1 < gestor->filaMax &&
                gestor->laberinto[fila-1][columna].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila-1][columna].direccionesCaminadas, ARRIBA)){
                    creaHilo(gestor,ARRIBA,fila-1, columna);
                }
                columna--;
                break;
            case DERECHA:
                if(fila+1 >= 0 &&
                fila+1 < gestor->filaMax &&
                gestor->laberinto[fila+1][columna].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila+1][columna].direccionesCaminadas, ABAJO)){
                    creaHilo(gestor,ABAJO,fila+1, columna);
                }
                if(fila-1 >= 0 &&
                fila-1 < gestor->filaMax &&
                gestor->laberinto[fila-1][columna].tipo==PASILLO &&
                !buscar(gestor->laberinto[fila-1][columna].direccionesCaminadas, ARRIBA)){
                    creaHilo(gestor,ARRIBA,fila-1, columna);
                }
                columna++;
                break;
        }

    }
    if (fila >= 0 &&
     fila < gestor->filaMax &&
     columna >= 0 && 
     columna < gestor->columnaMax && 
     gestor->laberinto[fila][columna].tipo == META && 
     !buscar(gestor->laberinto[fila][columna].direccionesCaminadas, direccion) &&
     !buscar(gestor->laberinto[fila][columna].direccionesCaminadas, direccionOpuesta)
     ){
        printf("EL HILO %d LLEGO A LA SALIDA DEL LABERNTO EN LA FILA %d Y COLUMNA %d",gestor->numeroHilos, fila, columna);
     }
}
void creaHilo(struct GestorLaberinto* gestor,int direccion,int filaActual, int columnaActual){
    gestor->numeroHilos++;
    gestor->arg[gestor->numeroHilos].idHilo=gestor->numeroHilos;
    gestor->arg[gestor->numeroHilos].direccion=direccion;
    gestor->arg[gestor->numeroHilos].gestor=gestor;
    gestor->arg[gestor->numeroHilos].filaActual=filaActual;
    gestor->arg[gestor->numeroHilos].columnaActual=columnaActual;

    pthread_create(&gestor->hilos[gestor->numeroHilos], NULL, (void* (*)(void*))moverse, (void*)&gestor->arg[gestor->numeroHilos]);
}

// Función para imprimir el laberinto periódicamente
void* imprimir_laberinto_periodicamente(void* arg) {
    struct GestorLaberinto* gestor = (struct GestorLaberinto*)arg;

    while (1) {
        printf("Laberinto actual:\n");
        pthread_mutex_lock(&gestor->mutex); // Bloquear el mutex antes de acceder a la estructura compartida
        for (int i = 0; i < gestor->filaMax; i++) {
            for (int j = 0; j < gestor->columnaMax; j++) {
                printf("%c", gestor->laberinto[i][j].elemento);
            }
            printf("\n");
        }
        printf("\n");
        pthread_mutex_unlock(&gestor->mutex);
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
            laberinto[i][j].direccionesCaminadas = crear_diccionario();
            if (letra == '*')
                laberinto[i][j].tipo = MURO;
            else if (letra == ' ')
                laberinto[i][j].tipo = PASILLO;
            else if (letra == '/')
                laberinto[i][j].tipo = META;
        }
        fgetc(archivo);
    }

    gestor->laberinto = laberinto;
    fclose(archivo);
    return gestor;
}


// Definiciones de constantes y estructuras omitidas por brevedad

void liberar_laberinto(struct GestorLaberinto* gestor) {
    for (int i = 0; i < gestor->filaMax; i++) {
        for (int j = 0; j < gestor->columnaMax; j++) {
            // Liberar la memoria de direccionesCaminadas en cada Casilla
            liberar_diccionario(gestor->laberinto[i][j].direccionesCaminadas);
        }
        // Liberar las filas del laberinto
        free(gestor->laberinto[i]);
    }
    // Liberar el laberinto
    free(gestor->laberinto);
    // Liberar el gestorLaberinto
    free(gestor);
}

int main() {
    const char* nombre_archivo = "laberinto.txt";

    struct GestorLaberinto* gestorLaberinto;


    gestorLaberinto = leer_laberinto(nombre_archivo);

    printf("Dimensiones del laberinto:\n");
    printf("Filas: %d\n", gestorLaberinto->filaMax);
    printf("Columnas: %d\n", gestorLaberinto->columnaMax);

    gestorLaberinto->intervalo_segundos = 1;
    if (pthread_mutex_init(&gestorLaberinto->mutex, NULL) != 0) {
        printf("Error al inicializar el mutex\n");
        exit(EXIT_FAILURE);
    }
    pthread_t thread_impresion;
    pthread_create(&thread_impresion, NULL, imprimir_laberinto_periodicamente, (void*)gestorLaberinto);

    // Crear hilo para iniciar el recorrido del laberinto
    gestorLaberinto->arg= malloc(4*gestorLaberinto->filaMax*gestorLaberinto->columnaMax*sizeof(struct HiloArgumentos));
    gestorLaberinto->hilos = malloc(4*gestorLaberinto->filaMax*gestorLaberinto->columnaMax*sizeof(pthread_t));
    gestorLaberinto->numeroHilos=-1;
    creaHilo(gestorLaberinto,ABAJO,0, 0);
    pthread_join(thread_impresion, NULL);
    free(gestorLaberinto->hilos);
    // Liberar la memoria asignada
    liberar_laberinto(gestorLaberinto);
    

    return 0;
}
