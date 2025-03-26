#include "estructuras.h"
#include "proxy-mq.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

Peticion peticion;

// Ejemplo 1 -> set_value
int main (int argc, char **argv){
  // Valores a asignar a la peticion
  int key = 1; 
  char *v1 = "ejemplo de valor 1";
  double v2[] = {2.3, 0.5, 23.45}; 
  struct Coord v3;
  v3.x = 10; 
  v3.y = 5;
  
  // Envio de peticion
  int mensaje = set_value(key, v1, 3, v2, v3);
 
  return mensaje; 
}