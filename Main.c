#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h> // Solo para comparación, pero no se usa en cálculos.

#define SIZE 10

// Declaración de estructuras y tipos
typedef struct {
    int base;
    int exp;
    int potencia;
} potencia_t;

typedef potencia_t *potenciaP_t;

// Función para establecer base y exponente
void setBaseExp(potenciaP_t p, int base, int exp) {
    p->base = base;
    p->exp = exp;
    p->potencia = -1;
}

// Función para calcular la potencia (base^exp)
int getPotencia(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// Función para asignar la potencia calculada en la estructura
void setPotenciaEst(potenciaP_t p) {
    p->potencia = getPotencia(p->base, p->exp);
}

// Inicializar array de estructuras
void initArrayEst(potencia_t arr[]) {
    for (int i = 0; i < SIZE; i++) {
        arr[i].base = i + 1;
        arr[i].exp = 0;
        arr[i].potencia = 1;
    }
}

// Mostrar array de estructuras
void printArrayEst(potencia_t arr[]) {
    for (int i = 0; i < SIZE; i++) {
        printf("arr[%d]: base: %d exp: %d potencia: %d\n", i, arr[i].base, arr[i].exp, arr[i].potencia);
    }
}

// Función para cálculo de potencia usando hebras
void *calcuPotHeb(void *arg) {
    potenciaP_t p = (potenciaP_t)arg;
    p->potencia = getPotencia(p->base, p->exp);
    return NULL;
}

int main() {
    potencia_t arr1[SIZE];
    potencia_t arr2[SIZE];

    // Inicializar y mostrar arr1
    initArrayEst(arr1);
    printf("Array arr1 inicializado:\n");
    printArrayEst(arr1);

    // Modificar base y exp en arr1
    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr1[i], i, 2);
    }
    printf("\nArray arr1 después de modificar base y exp:\n");
    printArrayEst(arr1);

    // Crear procesos hijos para calcular potencias
    for (int i = 0; i < SIZE; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Proceso hijo
            int potencia = getPotencia(arr1[i].base, arr1[i].exp);
            exit(potencia); // Devolver potencia calculada
        } else { // Proceso padre
            int status;
            wait(&status);
            arr1[i].potencia = WEXITSTATUS(status);
        }
    }
    printf("\nArray arr1 después de cálculo en procesos:\n");
    printArrayEst(arr1);

    // Inicializar y mostrar arr2
    initArrayEst(arr2);
    printf("\nArray arr2 inicializado:\n");
    printArrayEst(arr2);

    // Crear hebras para calcular potencias en arr2
    pthread_t threads[SIZE];
    for (int i = 0; i < SIZE; i++) {
        arr2[i].exp = 2;
        pthread_create(&threads[i], NULL, calcuPotHeb, &arr2[i]);
    }

    // Esperar a que terminen las hebras
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\nArray arr2 después de cálculo en hebras:\n");
    printArrayEst(arr2);

    return 0;
}
