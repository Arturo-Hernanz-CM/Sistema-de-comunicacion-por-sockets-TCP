#include "estructuras.h"
#include "proxy-mq.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

Peticion peticion;

// Ejemplo 3 -> modify_value
int main (int argc, char **argv){
  // Valores a asignar a la peticion
  int key = 1; 
  char *v1 = "ejemplo de valor 2";
  double v2[] = {1.0, 0.5}; 
  struct Coord v3;
  v3.x = 5; 
  v3.y = 10; 
  
  // Envio de peticion
  int mensaje = modify_value(key, v1, 2, v2, v3);
  
  return mensaje;  
}