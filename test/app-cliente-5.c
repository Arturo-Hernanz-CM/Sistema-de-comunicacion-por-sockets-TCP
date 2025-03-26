#include "estructuras.h"
#include "proxy-mq.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

Peticion peticion;

// Ejemplo 5 -> exist
int main (int argc, char **argv){
  // Valores a asignar a la peticion
  int key = 1;
  
  // Envio de peticion
  int mensaje = exist(key); 
  
  return mensaje; 
}