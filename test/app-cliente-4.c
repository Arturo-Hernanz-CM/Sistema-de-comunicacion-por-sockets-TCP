#include "estructuras.h"
#include "proxy-mq.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

Peticion peticion;

// Ejemplo 4 -> delete_key
int main (int argc, char **argv){
  // Valores a asignar a la peticion
  int key = 1;

  // Envio de peticion
  int mensaje = delete_key(key); 

  return mensaje; 
}