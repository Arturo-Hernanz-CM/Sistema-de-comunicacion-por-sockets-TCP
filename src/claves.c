#include "estructuras.h"
#include "claves.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

// Variable global
extern Lista *base_datos;

int destroy(void){
    if(base_datos != NULL){
        free(base_datos); 
        base_datos = NULL;

        base_datos = (Lista *) malloc(sizeof(Lista));
        if (base_datos == NULL) {
            perror("ERROR:\nFile: servidor-mq.c\nFunction: main\nError: Error al reservar memoria para la lista de claves");
            return -1;
        }
        base_datos->inicio = NULL;
        base_datos->final = NULL;
        base_datos->size = 0;
    }
    printf("Lista destruida.\n");
    return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    if(N_value2 < 1 || N_value2 > 32){
        perror("ERROR:\nFile: claves.c\nFunction: set_value\nError: El valor N_value2 debe estar entre 1 y 32");
        return -1;
    }

    Nodo *nodo = (Nodo *) malloc(sizeof(Nodo));
    if (nodo == NULL) {
        perror("ERROR:\nFile: claves.c\nFunction: set_value\nError: Error al reservar memoria para guardar un nodo");
        return -1;
    }

    nodo->key = key;
    nodo->value1 = (char*)malloc(strlen(value1)+1);
    if (nodo->value1 == NULL) {
        perror("ERROR:\nFile: claves.c\nFunction: set_value\nError: Error al reservar memoria para guardar el value1");
        return -1;
    }
    strcpy(nodo->value1, value1);

    nodo->N_value2 = N_value2;

    nodo->value2 = (double*)malloc(N_value2*sizeof(double));
    if (nodo->value2 == NULL) {
        perror("ERROR:\nFile: claves.c\nFunction: set_value\nError: Error al reservar memoria para guardar el value2");
        return -1;
    }
    memcpy(&nodo->value2, V_value2, sizeof(double));
    nodo->value3 = value3;

    if (base_datos->size == 0){ // lista vacia
        nodo->siguiente = NULL;
        nodo->index = 0;

        base_datos = (Lista *) malloc(sizeof(Lista));
        if (base_datos == NULL) {
            perror("ERROR:\nFile: claves.c\nFunction: set_value\nError: Error al reservar memoria para la lista de claves");
            return -1;
        }

        base_datos->inicio = nodo;
        base_datos->final = nodo;
        base_datos->size = 1;
    }
    else{
        if(exist(key) == 0){
            perror("ERROR:\nFile: claves.c\nFunction: set_value\nError: La clave ya existe en la base de datos");
            return -1;
        }
        nodo->siguiente = NULL;
        nodo->index = base_datos->size;
        base_datos->final = nodo;
        base_datos->final->siguiente = NULL;
        base_datos->size++;
    }
    printf("Mensaje guardado en la lista.\n");
    return 0;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    if (exist(key) == -1){
        perror("ERROR:\nFile: claves.c\nFunction: get_value\nError: La clave no existe");
        return -1;
    }
    else{
        Nodo *nodo_actual = base_datos->inicio;
        for(int indice = 0; indice < base_datos->size; indice++){
            if(nodo_actual->key == key){
                strcpy(value1, nodo_actual->value1);
                memcpy(N_value2, &nodo_actual->N_value2, sizeof(nodo_actual->N_value2));
                memcpy(V_value2, &nodo_actual->value2, sizeof(nodo_actual->value2));
                value3->x = nodo_actual->value3.x;
                value3->y = nodo_actual->value3.y;
                printf("Datos recuperados.\n");
                return 0;
            }
            else{
                nodo_actual = nodo_actual->siguiente;
            }
        }
        return -1;
    }
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    if (N_value2 < 1 || N_value2 > 32){
        perror("ERROR:\nFile: claves.c\nFunction: modify_value\nError: El valor N_value2 debe estar entre 1 y 32");
        return -1;
    }
    if (exist(key) == -1){
        perror("ERROR:\nFile: claves.c\nFunction: modify_value\nError: La clave no existe");
        return -1;
    }
    else{
        Nodo *nodo_actual = base_datos->inicio;
        for(int indice = 0; indice < base_datos->size; indice++){
            if(nodo_actual->key == key){
                nodo_actual->value1 = (char*)malloc(strlen(value1)+1);
                if (nodo_actual->value1 == NULL) {
                    return -1;
                }
                else{
                    strcpy(nodo_actual->value1, value1);
                }
                nodo_actual->N_value2 = N_value2;
                nodo_actual->value2 = V_value2;
                nodo_actual->value3 = value3;
                printf("Datos modificados.\n");
                return 0;
            }
            else{
                nodo_actual = nodo_actual->siguiente;
            }
        }
    }

    return 0;
}

int delete_key(int key){
    if (exist(key) == -1){
        perror("ERROR:\nFile: claves.c\nFunction: delete_key\nError: La clave no existe");
        return -1;
    }
    else{
        if(base_datos->size == 1){
            free(base_datos->inicio);
            base_datos->inicio = NULL;
            base_datos->final = NULL;
            base_datos->size = 0;
        }
        else{
            Nodo *nodo_actual = base_datos->inicio;
            Nodo *nodo_anterior = NULL;
            for(int indice = 0; indice < base_datos->size; indice++){
                if(nodo_actual->key == key){
                    if(nodo_anterior == NULL){
                        base_datos->inicio = nodo_actual->siguiente;
                    }
                    else if(nodo_actual == base_datos->final){
                        base_datos->final = nodo_anterior;
                    }
                    else{
                        nodo_anterior->siguiente = nodo_actual->siguiente;
                    }
                    base_datos->size--;
                    free(nodo_actual);
                }
                else{
                    nodo_anterior = nodo_actual;
                    nodo_actual = nodo_actual->siguiente;
                }
            }
        }
        printf("Datos asociados a la clave destruidos.\n");
        return 0;
    }
}

int exist(int key){
    if (base_datos == NULL){
        perror("ERROR:\nFile: claves.c\nFunction: exist\nError: La lista esta vacia");
        return -1;
    }
    else{
        Nodo *nodo_actual = base_datos->inicio;
        for(int indice = 0; indice < base_datos->size; indice++){
            if(nodo_actual->key == key){
                printf("Clave encontrada.\n");
                return 0;
            }
            else{
                if(nodo_actual != base_datos->final){
                    nodo_actual = nodo_actual->siguiente;
                }
                else{
                    return -1;
                }
            }
        }
        return -1;
    }
}