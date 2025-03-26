#ifndef proxymq_h
#define proxymq_h

#include "estructuras.h"

int destroy(void);
int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);
int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3);
int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);
int delete_key(int key);
int exist(int key);
int tratar_peticion(Peticion * peticion);

#endif