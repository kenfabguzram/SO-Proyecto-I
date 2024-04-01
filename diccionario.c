#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TABLE_SIZE 4

// Definición de una estructura para el elemento del diccionario
struct elemento {
    int clave;
    bool valor;
    struct elemento* siguiente;
};

// Definición de la tabla hash
struct diccionario {
    struct elemento* tabla[TABLE_SIZE];
};

// Función de hash simple para convertir la clave en un índice dentro de la tabla
int hash(int clave) {
    return clave % TABLE_SIZE;
}

// Función para crear un nuevo elemento del diccionario
struct elemento* crear_elemento(int clave, bool valor) {
    struct elemento* nuevo_elemento = malloc(sizeof(struct elemento));
    if (nuevo_elemento == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para el elemento del diccionario.\n");
        exit(EXIT_FAILURE);
    }
    nuevo_elemento->clave = clave;
    nuevo_elemento->valor = valor;
    nuevo_elemento->siguiente = NULL;
    return nuevo_elemento;
}

// Función para insertar un par clave-valor en el diccionario
void insertar(struct diccionario* dict, int clave, bool valor) {
    int indice = hash(clave);
    struct elemento* actual = dict->tabla[indice];
    struct elemento* previo = NULL;

    // Buscar si la clave ya existe en la tabla hash
    while (actual != NULL) {
        if (actual->clave == clave) {
            // La clave ya existe, actualizamos el valor
            actual->valor = valor;
            return;
        }
        previo = actual;
        actual = actual->siguiente;
    }

    // La clave no existe, insertamos un nuevo elemento
    struct elemento* nuevo_elemento = crear_elemento(clave, valor);
    if (previo == NULL) {
        // Si la lista enlazada está vacía en este índice
        dict->tabla[indice] = nuevo_elemento;
    } else {
        previo->siguiente = nuevo_elemento;
    }
}

// Función para buscar un valor asociado a una clave en el diccionario
bool buscar(struct diccionario* dict, int clave) {
    int indice = hash(clave);
    struct elemento* actual = dict->tabla[indice];

    while (actual != NULL) {
        if (actual->clave == clave) {
            // Se encontró la clave, devolvemos el valor asociado
            return actual->valor;
        }
        actual = actual->siguiente;
    }

    // Si no se encuentra la clave, devolvemos un valor predeterminado
    return false; // Por ejemplo, podrías devolver un valor especial para indicar que la clave no se encontró
}

// Función para liberar la memoria asignada al diccionario
void liberar_diccionario(struct diccionario* dict) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct elemento* actual = dict->tabla[i];
        while (actual != NULL) {
            struct elemento* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
    }
    free(dict);
}
void cambiar_valor(struct diccionario* dict, int clave, bool nuevo_valor) {
    int indice = hash(clave);
    struct elemento* actual = dict->tabla[indice];

    while (actual != NULL) {
        if (actual->clave == clave) {
            // Se encontró la clave, actualizamos el valor
            actual->valor = nuevo_valor;
            return;
        }
        actual = actual->siguiente;
    }

    // Si no se encuentra la clave, no hacemos nada
    // Podrías optar por insertar la clave con el nuevo valor en lugar de no hacer nada
}
/*
int main() {
    // Crear el diccionario
    struct diccionario* dict = malloc(sizeof(struct diccionario));
    if (dict == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para el diccionario.\n");
        exit(EXIT_FAILURE);
    }

    // Insertar elementos en el diccionario
    insertar(dict, 1, true);
    insertar(dict, 2, false);
    insertar(dict, 3, true);

    // Buscar elementos en el diccionario
    printf("Valor asociado a clave 1: %s\n", buscar(dict, 1) ? "true" : "false");
    printf("Valor asociado a clave 2: %s\n", buscar(dict, 2) ? "true" : "false");
    printf("Valor asociado a clave 3: %s\n", buscar(dict, 3) ? "true" : "false");
    printf("Valor asociado a clave 4: %s\n", buscar(dict, 4) ? "true" : "false"); // Clave no existente

    // Liberar memoria
    liberar_diccionario(dict);

    return 0;
}
*/