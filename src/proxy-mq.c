#include "proxy-mq.h"
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>


// Variable global
Peticion peticion = {0};

// Funciones

int tratar_peticion(Peticion * peticion) {

    // Variables
    char nombre_cola[256];
    int respuesta;

    int socket_cliente;
	struct sockaddr_in socket_servidor;	// socket del servidor
	struct hostent *datos_servidor;	// datos del servidor
	int32_t a, b, res;
	int error;
    
    // Socket del cliente
	socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_cliente == 1) {
		printf("Error en socket\n");
		return -1;
	}

    // Socket del servidor
	bzero((char *)&socket_servidor, sizeof(socket_servidor));

   	datos_servidor = gethostbyname (argv[1]);
	if (datos_servidor == NULL) {
		printf("Error en gethostbyname\n");
		return -1;
	}
    
    // conectar el socket del cliente con el servidor
    error = connect(socket_cliente, (struct sockaddr *) &datos_servidor,  sizeof(socket_servidor));
	if (error == -1) {
		printf("Error en connect\n");
		return -1;
	}

    // Enviar datos al servidor
    printf("Enviando peticion.\n");
    error = sendMessage(socket_cliente, (char *) &op, sizeof(char));  // envía la operacion
	if (err == -1){
		printf("Error en envio\n");
		return -1;
	}

    // Recibir respuesta en la cola del cliente
    if (peticion->operacion == 2){
        Peticion resp;
        printf("Recibiendo respuesta del servidor.\n");
        if(mq_receive(cola_cliente, (char *)&resp, sizeof(Peticion), 0) == -1) {
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: No se puede recibir la respuesta");
            mq_close(cola_cliente);
            mq_unlink(nombre_cola);
            return -1;
        }
        if (resp.N_value2 == -1){
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error en la respuesta");
            mq_close(cola_cliente);
            mq_unlink(nombre_cola);
            return -1;
        }
        strcpy(peticion->value1, resp.value1);
        peticion->N_value2 = resp.N_value2;
        memcpy(&peticion->value2, &resp.value2, sizeof(double));
        peticion->value3 = resp.value3;
    }
    else{
        printf("Recibiendo respuesta del servidor.\n");
        if(mq_receive(cola_cliente, (char *)&respuesta, sizeof(respuesta), 0) == -1) {
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: No se puede recibir la respuesta");
            mq_close(cola_cliente);
            mq_unlink(nombre_cola);
            return -1;
        }
        if (respuesta == -1){
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error en la respuesta");
            mq_close(cola_cliente);
            mq_unlink(nombre_cola);
            return -1;
        }
    }
    
	mq_close(cola_servidor);
	mq_close(cola_cliente);
    mq_unlink(nombre_cola);

    return 0;
}

int destroy(void){
    // Valores a asignar a la peticion
    struct Coord value3 = {0, 0};

    // Asignacion de valores a la estructura
    peticion.key = 0;
    peticion.N_value2 = 0;
    peticion.value3 = value3;
    peticion.operacion = 0;

    if (tratar_peticion(&peticion) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: destroy\nError: Error al destruir");
        return -1;
    }

    printf("Lista destruida.\n");
    return 0;
}
int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    // Asignacion de valores a la estructura
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(&peticion.value2, V_value2, sizeof(double));
    peticion.value3 = value3;
    peticion.operacion = 1;

    if (tratar_peticion(&peticion) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: set_value\nError: Error al hacer set");
        return -1;
    }

    printf("Mensaje guardado en la lista.\n");
    return 0;
}
int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    // Asignacion de valores a la estructura
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = 0;
    peticion.value3 = *value3;
    memcpy(&peticion.value3, value3, sizeof(struct Coord));
    peticion.operacion = 2;
    
    if (tratar_peticion(&peticion) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: get_value\nError: Error al hacer get");
        return -1;
    }

    // Escribir la respuesta en los valores que se pasan como argumentos
    strcpy(value1, peticion.value1);
    *N_value2 = peticion.N_value2;
    memcpy(V_value2, &peticion.value2, sizeof(double));
    *value3 = peticion.value3;

    printf("Datos de la clave %d recuperados:\n", key);
    printf("value1: %s\n", value1);
    printf("N_value2: %d\n", *N_value2);
    printf("value3.x: %d\n", value3->x);
    printf("value3.y: %d\n", value3->y);
    
    return 0;
}
int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    // Asignacion de valores a la estructura
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = 2;
    memcpy(&peticion.value2, V_value2, sizeof(double));
    peticion.value3 = value3;
    peticion.operacion = 3;

    if (tratar_peticion(&peticion) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: modify_value\nError: Error al modificar");
        return -1;
    }

    printf("Datos modificados.\n");
    return 0;
}
int delete_key(int key){
    // Valores a asignar a la peticion
    struct Coord value3 = {0, 0};

    // Asignacion de valores a la estructura
    peticion.key = key;
    peticion.N_value2 = 0;
    peticion.value3 = value3;
    peticion.operacion = 4;
    
    if (tratar_peticion(&peticion) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: delete_key\nError: Error al borrar");
        return -1;
    }

    printf("Datos asociados a la clave destruidos.\n");
    return 0;
}
int exist(int key){
    // Valores a asignar a la peticion
    struct Coord value3 = {0, 0};

    // Asignacion de valores a la estructura
    peticion.key = key;
    peticion.N_value2 = 0;
    peticion.value3 = value3;
    peticion.operacion = 5; 

    if (tratar_peticion(&peticion) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: exist\nError: Error al comprobar existencia");
        return -1;
    }

    printf("Clave encontrada.\n");
    return 0;
}