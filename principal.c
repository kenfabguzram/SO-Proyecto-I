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

#define MAX_HILOS 50

struct Casilla
{
    struct diccionario *direccionesCaminadas;
    int posicionMatriz;
    int tipo;
    int fila;
    int columna;
    char elemento;
};

struct GestorLaberinto
{
    struct Casilla **laberinto;
    int filaMax;
    int columnaMax;
    int intervalo_segundos;
    struct hiloArgumentos *hilos;
    int contadorHilos;
    int direccion;
};

struct hiloArgumentos
{
    int direccion;
    int filaActual;
    int columnaActual;
    bool terminado;
    int numeroHilo;
    pthread_t hilo;
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
void *imprimir_laberinto_periodicamente(void *arg)
{
    struct GestorLaberinto *gestor = (struct GestorLaberinto *)arg;

    while (1)
    {
        printf("Laberinto actual:\n");
        for (int i = 0; i < gestor->filaMax; i++)
        {
            for (int j = 0; j < gestor->columnaMax; j++)
            {
                printf("%c", gestor->laberinto[i][j].elemento);
            }
            printf("\n");
        }
        printf("\n");

        sleep(gestor->intervalo_segundos);
    }

    return NULL;
}

struct GestorLaberinto *leer_laberinto(const char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    struct GestorLaberinto *gestor = malloc(sizeof(struct GestorLaberinto));
    if (gestor == NULL)
    {
        printf("Error al asignar memoria.\n");
        exit(EXIT_FAILURE);
    }

    int fila, columna;
    fscanf(archivo, "%d %d", &fila, &columna);
    gestor->filaMax = fila;
    gestor->columnaMax = columna;
    gestor->direccion = DERECHA;
    gestor->contadorHilos = 0;

    struct hiloArgumentos *hilos = malloc(MAX_HILOS * sizeof(struct hiloArgumentos));
    gestor->hilos = hilos;

    fgetc(archivo);

    struct Casilla **laberinto = malloc(fila * sizeof(struct Casilla *));
    if (laberinto == NULL)
    {
        printf("Error al asignar memoria.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fila; i++)
    {
        laberinto[i] = malloc(columna * sizeof(struct Casilla));
        if (laberinto[i] == NULL)
        {
            printf("Error al asignar memoria.\n");
            exit(EXIT_FAILURE);
        }
    }

    char letra;
    for (int i = 0; i < fila; i++)
    {
        for (int j = 0; j < columna; j++)
        {
            letra = fgetc(archivo);
            laberinto[i][j].elemento = letra;
            laberinto[i][j].fila = i;
            laberinto[i][j].columna = j;
            laberinto[i][j].direccionesCaminadas = malloc(sizeof(struct diccionario));
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

bool revisar_adjacentes_t(int row, int column, struct GestorLaberinto *gestorLaberinto)
{
    int filaMax = gestorLaberinto->filaMax;
    struct Casilla **laberinto = gestorLaberinto->laberinto;
    if (row - 1 < 0)
    {
        return false;
    }

    return true;
}

bool revisar_adjacentes_b(int row, int column, struct GestorLaberinto *gestorLaberinto)
{
    int filaMax = gestorLaberinto->filaMax;
    struct Casilla **laberinto = gestorLaberinto->laberinto;
    if (row + 1 >= filaMax)
    {
        return false;
    }

    return true;
}

bool revisar_adjacentes_i(int row, int column, struct GestorLaberinto *gestorLaberinto)
{
    int columnaMax = gestorLaberinto->columnaMax;
    struct Casilla **laberinto = gestorLaberinto->laberinto;
    if (column - 1 < 0)
    {
        return false;
    }

    return true;
}

bool revisar_adjacentes_d(int row, int column, struct GestorLaberinto *gestorLaberinto)
{
    int columnaMax = gestorLaberinto->columnaMax;
    struct Casilla **laberinto = gestorLaberinto->laberinto;
    if (column + 1 >= columnaMax)
    {
        return false;
    }

    return true;
}

// Codigo de los hilos
void *creador(void *arg)
{
    struct GestorLaberinto *gestor = (struct GestorLaberinto *)arg;
    struct Casilla **laberinto = (struct Casilla **)gestor->laberinto;
    int filaActual = gestor->hilos[gestor->contadorHilos].filaActual;
    int columnaActual = gestor->hilos[gestor->contadorHilos].columnaActual;
    int direccion = gestor->hilos[gestor->contadorHilos].direccion;
    bool terminado = gestor->hilos[gestor->contadorHilos].terminado;
    int numeroHilo = gestor->contadorHilos;
    struct hiloArgumentos *hiloQueue = malloc(2 * sizeof(struct hiloArgumentos));
    bool newHilo1 = false;
    bool newHilo2 = false;
    char elem = ' '; 

    if (direccion == ARRIBA || direccion == ABAJO)
    {
        elem = 'y';
    }
    else if (direccion == IZQUIERDA || direccion == DERECHA)
    {
        elem = 'x';
    }

    while (!terminado)
    {
        struct Casilla *casillaActual = &laberinto[filaActual][columnaActual];
        if (casillaActual->tipo == MURO)
        {
            printf("I've hit a wall at: %d %d\n", filaActual, columnaActual);
            terminado = true;
        }
        else if (casillaActual->tipo == META)
        {
            printf("I've reached the end\n");
            terminado = true;
        }

        if(laberinto[filaActual][columnaActual].tipo != MURO && laberinto[filaActual][columnaActual].tipo != META){
            laberinto[filaActual][columnaActual].elemento = elem;
        }

        if (direccion == ARRIBA || direccion == ABAJO)
        {
            if (revisar_adjacentes_i(filaActual, columnaActual, gestor))
            {
                struct Casilla *casillaIzquierda = &laberinto[filaActual][columnaActual - 1];
                if (casillaIzquierda->tipo == PASILLO)
                {
                    hiloQueue[0].direccion = IZQUIERDA;
                    hiloQueue[0].filaActual = filaActual;
                    hiloQueue[0].columnaActual = columnaActual - 1;
                    hiloQueue[0].terminado = false;
                    newHilo1 = true;
                    printf("Espacio en casilla izquierda de: %d %d\n", filaActual, columnaActual);
                }
            }
            if (revisar_adjacentes_d(filaActual, columnaActual, gestor))
            {
                struct Casilla *casillaDerecha = &laberinto[filaActual][columnaActual + 1];
                if (casillaDerecha->tipo == PASILLO)
                {
                    hiloQueue[1].direccion = DERECHA;
                    hiloQueue[1].filaActual = filaActual;
                    hiloQueue[1].columnaActual = columnaActual + 1;
                    hiloQueue[1].terminado = false;
                    newHilo2 = true;
                    printf("Espacio en casilla derecha de: %d %d\n", filaActual, columnaActual);
                }
            }
        }
        else if (direccion == IZQUIERDA || direccion == DERECHA)
        {
            if (revisar_adjacentes_t(filaActual, columnaActual, gestor))
            {
                struct Casilla *casillaArriba = &laberinto[filaActual - 1][columnaActual];
                if (casillaArriba->tipo == PASILLO)
                {
                    hiloQueue[0].direccion = ARRIBA;
                    hiloQueue[0].filaActual = filaActual - 1;
                    hiloQueue[0].columnaActual = columnaActual;
                    hiloQueue[0].terminado = false;
                    newHilo1 = true;
                    printf("Espacio en casilla arriba de: %d %d\n", filaActual, columnaActual);
                }
            }
            if (revisar_adjacentes_b(filaActual, columnaActual, gestor))
            {
                struct Casilla *casillaAbajo = &laberinto[filaActual + 1][columnaActual];
                if (casillaAbajo->tipo == PASILLO)
                {
                    hiloQueue[1].direccion = ABAJO;
                    hiloQueue[1].filaActual = filaActual + 1;
                    hiloQueue[1].columnaActual = columnaActual;
                    hiloQueue[1].terminado = false;
                    newHilo2 = true;
                    printf("Espacio en casilla abajo de: %d %d\n", filaActual, columnaActual);
                }
            }
        }

        pthread_mutex_t mutex; // Declare a mutex variable

        if (newHilo1 || newHilo2)
        {
            pthread_mutex_lock(&mutex); // Enter the critical zone
            if (newHilo1)
            {
                gestor->contadorHilos++;
                hiloQueue[0].numeroHilo = gestor->contadorHilos;
                gestor->hilos[gestor->contadorHilos] = hiloQueue[0];
                lilith((void *)gestor);
                newHilo1 = false;
            }
            if (newHilo2)
            {
                gestor->contadorHilos++;
                hiloQueue[1].numeroHilo = gestor->contadorHilos;
                gestor->hilos[gestor->contadorHilos] = hiloQueue[1];
                lilith((void *)gestor);
                newHilo2 = false;  
            }
            pthread_mutex_unlock(&mutex); // Exit the critical zone
        }

        switch (direccion)
        {
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
        // Revisar que la siguiente casilla esta dentro de los limites del laberinto
        if (filaActual < 0 || filaActual >= gestor->filaMax || columnaActual < 0 || columnaActual >= gestor->columnaMax)
        {
            terminado = true;
        }
        sleep(2);
    }

    return NULL;
}

//Creador de nuevos hilos
void lilith(void *arg){
    struct GestorLaberinto *gestor = (struct GestorLaberinto *)arg;
    struct hiloArgumentos hilo = gestor->hilos[gestor->contadorHilos];
    pthread_t thread_nuevo;
    gestor->hilos[gestor->contadorHilos].hilo = thread_nuevo;
    pthread_create(&thread_nuevo, NULL, creador, (void *)gestor);

    for(int i = 0; i < MAX_HILOS; i++){
        if(gestor->hilos[i].terminado){
            pthread_join(gestor->hilos[i].hilo, NULL);
            gestor->contadorHilos--;
        }
    }
};

int main()
{
    const char *nombre_archivo = "laberinto.txt";

    struct GestorLaberinto *gestorLaberinto;

    gestorLaberinto = leer_laberinto(nombre_archivo);

    printf("Dimensiones del laberinto:\n");
    printf("Filas: %d\n", gestorLaberinto->filaMax);
    printf("Columnas: %d\n", gestorLaberinto->columnaMax);

    gestorLaberinto->intervalo_segundos = 5;

    struct hiloArgumentos *hilo = malloc(sizeof(struct hiloArgumentos));
    hilo->direccion = DERECHA;
    hilo->filaActual = 0;
    hilo->columnaActual = 0;
    hilo->terminado = false;
    hilo->numeroHilo = 0;
    gestorLaberinto->hilos[0] = *hilo;

    pthread_t thread_impresion;
    pthread_create(&thread_impresion, NULL, imprimir_laberinto_periodicamente, (void *)gestorLaberinto);

    pthread_t thread_creador;
    gestorLaberinto->hilos[0].hilo = thread_creador;
    pthread_create(&thread_creador, NULL, creador, (void *)gestorLaberinto);

    pthread_join(thread_creador, NULL);
    pthread_join(thread_impresion, NULL);

    return 0;
}
