#include "../estructuras.h"
#include "../proxy-sock.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

// Ejemplo 0 -> destroy
int main (int argc, char **argv){
   
  // Envio de peticion 
  int mensaje = destroy();
 
  return mensaje; 
}