#include "estructuras.h"
#include "proxy-mq.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

// Ejemplo 2 -> get_value
int main (int argc, char **argv){
  // Valores a asignar a la peticion
  int key = 1;
  char *value1 = NULL;
  int *N_value2 = NULL;
  double *value2 = NULL;
  struct Coord * value3 = NULL;

  // Reserva de memoria
  value1 = (char *)malloc(256);
  if (value1 == NULL) {
    perror("ERROR:\nFile: app-cliente2.c\nFunction: main\nError al reservar memoria para guardar el value1");
    return -1;
  }

  N_value2 = (int *)malloc(sizeof(int));
  if (N_value2 == NULL) {
    perror("ERROR:\nFile: app-cliente2.c\nFunction: main\nError al reservar memoria para guardar el N_value2");
    return -1;
  }

  value2 = (double *)malloc(sizeof(double));
  if (value2 == NULL) {
    perror("ERROR:\nFile: app-cliente2.c\nFunction: main\nError al reservar memoria para guardar el value2");
    return -1;
  }

  value3 = (struct Coord *)malloc(sizeof(struct Coord));
  if (value3 == NULL) {
    perror("ERROR:\nFile: app-cliente2.c\nFunction: main\nError al reservar memoria para guardar el value3");
    return -1;
  }

  // Envio de peticion
  int mensaje = get_value(key, value1, N_value2, value2, value3);

  printf("%s\n", value1);
  free(value1);
  free(N_value2);
  free(value2);
  free(value3);

  return mensaje; 
}