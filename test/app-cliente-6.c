#include "estructuras.h"
#include "proxy-mq.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

Peticion peticion;

// Ejemplo 6 -> set_value diferente
int main (int argc, char **argv){
  // Valores a asignar a la peticion
  int key = 2; 
  char *v1 = "ejemplo de valor 2";
  double v2[] = {2.333, 50.37, 23}; 
  struct Coord v3;
  v3.x = 14; 
  v3.y = 5;
  
  // Envio de peticion
  int mensaje = set_value(key, v1, 3, v2, v3);
 
  return mensaje; 
}