#include "claves.h"
#ifndef estructuras_h
#define estructuras_h

typedef struct Nodo{
    int index;
    
    int key;
    char *value1;
    int N_value2;
    double *value2;
    struct Coord value3; 

    struct Nodo *siguiente;
}Nodo;

typedef struct{
    Nodo *inicio;
    Nodo *final;
    int size;
}Lista;

typedef struct{
    int key;
    char value1[256];  
    int N_value2;
    double value2[32];
    struct Coord value3;
    int operacion;
    /*
        0 -> destroy
        1 -> set_value
        2 -> get_value
        3 -> modify_value
        4 -> delete_key
        5 -> exist
    */

    char cola_cliente[256];
}Peticion;

#endif