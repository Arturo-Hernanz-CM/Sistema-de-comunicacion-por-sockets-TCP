#include "proxy-mq.h"
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>


// Variable global
Peticion peticion = {0};
ip_servidor = getenv("IP_TUPLAS");
puerto_servidor_char = getenv("PORT_TUPLAS");
int puerto_servidor;

// Comprobacion de las variables de entorno
if (ip_servidor == NULL) {
    perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al obtener la variable de entorno IP_TUPLAS \n");
    return -1;
}

if (puerto_servidor_char == NULL) {
    perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al obtener la variable de entorno PORT_TUPLAS \n");
    return -1;
}
else{
    puerto_servidor = atoi(puerto_servidor_char);
    if(puerto_servidor == 0){
        if strcmp(puerto_servidor_char, "0") != 0 {
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al obtener el puerto del servidor\n");
            return -1;
        }
    }
}


// Funciones

int enviar_peticion(int socket_cliente, Peticion * peticion){
   int32_t key_, N_value2_, operacion_, value3_x_, value3_y_;
   key_ = htonl(peticion->key);
   N_value2_ = htonl(peticion->N_value2);
   operacion_ = htonl(peticion->operacion);
   value3_x_ = htonl(peticion->value3.x);
   value3_y_ = htonl(peticion->value3.y);

   // Enviar key
   if (sendMessage(socket_cliente, &key_, sizeof(key_), 0) == -1) {
        perror("ERROR:\nFile: proxy-mq.c\nFunction: enviar_peticion\nError: Error al enviar la clave\n");
        close(socket_cliente);
        return -1;
    }

    // Enviar N_value2
    if (sendMessage(socket_cliente, &N_value2_, sizeof(N_value2_), 0) == -1) {
        perror("ERROR:\nFile: proxy-mq.c\nFunction: enviar_peticion\nError: Error al enviar el N_value2\n");
        close(socket_cliente);
        return -1;
    }

    // Enviar operacion
    if (sendMessage(socket_cliente, &operacion_, sizeof(operacion_), 0) == -1) {
        perror("ERROR:\nFile: proxy-mq.c\nFunction: enviar_peticion\nError: Error al enviar la operacion\n");
        close(socket_cliente);
        return -1;
    }

    // Enviar value3.x
    if (sendMessage(socket_cliente, &value3_x_, sizeof(value3_x_), 0) == -1) {
        perror("ERROR:\nFile: proxy-mq.c\nFunction: enviar_peticion\nError: Error al enviar value3.x\n");
        close(socket_cliente);
        return -1;
    }

    // Enviar value3.y
    if (sendMessage(socket_cliente, &value3_y_, sizeof(value3_y_), 0) == -1) {
        perror("ERROR:\nFile: proxy-mq.c\nFunction: enviar_peticion\nError: Error al enviar value3.y\n");
        close(socket_cliente);
        return -1;
    }

    // Enviar value1
    if (sendMessage(socket_cliente, peticion->value1, sizeof(peticion->value1), 0) == -1) {
        perror("ERROR:\nFile: proxy-mq.c\nFunction: enviar_peticion\nError: Error al enviar value1\n");
        close(socket_cliente);
        return -1;
    }

    // Enviar value2
    if (sendMessage(socket_cliente, &peticion->value2, sizeof(peticion->value2), 0) == -1) {
        perror("ERROR:\nFile: proxy-mq.c\nFunction: enviar_peticion\nError: Error al enviar value2\n");
        close(socket_cliente);
        return -1;
    }

    return 0;
}

int recibir_get_value(int socket_cliente, Peticion * resp){
    int32_t key_, N_value2_, operacion_, value3_x_, value3_y_;

    // Recibir key
    if(rcvMessage(socket_cliente, &key_, sizeof(key_), 0) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: recibir_get_value\nError: Error al recibir la key\n");
        close(socket_cliente);
        return -1;
    }

    // Recibir N_value2
    if(rcvMessage(socket_cliente, &N_value2_, sizeof(N_value2_), 0) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: recibir_get_value\nError: Error al recibir el N_value2\n");
        close(socket_cliente);
        return -1;
    }

    // Recibir operacion
    if(rcvMessage(socket_cliente, &operacion_, sizeof(operacion_), 0) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: recibir_get_value\nError: Error al recibir la operacion\n");
        close(socket_cliente);
        return -1;
    }

    // Recibir value3.x
    if(rcvMessage(socket_cliente, &value3_x_, sizeof(value3_x_), 0) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: recibir_get_value\nError: Error al recibir value3.x\n");
        close(socket_cliente);
        return -1;
    }

    // Recibir value3.y
    if(rcvMessage(socket_cliente, &value3_y_, sizeof(value3_y_), 0) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: recibir_get_value\nError: Error al recibir value3.y\n");
        close(socket_cliente);
        return -1;
    }    

    // Recibir value1
    if(rcvMessage(socket_cliente, resp->value1, sizeof(resp->value1), 0) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: recibir_get_value\nError: Error al recibir value1\n");
        close(socket_cliente);
        return -1;
    }

    // Recibir value2
    if(rcvMessage(socket_cliente, &resp->value2, sizeof(resp->value2), 0) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: recibir_get_value\nError: Error al recibir value2\n");
        close(socket_cliente);
        return -1;
    }

    // Asignacion de valores a la estructura
    resp->key = ntohl(key_);
    resp->N_value2 = ntohl(N_value2_);
    resp->operacion = ntohl(operacion_);
    resp->value3.x = ntohl(value3_x_);
    resp->value3.y = ntohl(value3_y_);

    return 0;
}

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
	if (socket_cliente == -1) {
		perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al crear el socket del cliente\n");
		close(socket_cliente);
        return -1;
	}

    // Socket del servidor
	bzero((char *)&socket_servidor, sizeof(socket_servidor));
   	datos_servidor = gethostbyname (ip_servidor);
	if (datos_servidor == NULL) {
		perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al obtener la informacion del servidor\n");
		close(socket_cliente);
        return -1;
	}

    // Guardar los datos del servidr en la estructura para usarlos en el socket
    memcpy (&(socket_servidor.sin_addr), datos_servidor->h_addr, datos_servidor->h_length);
    socket_servidor.sin_family  = AF_INET;
    socket_servidor.sin_port    = htons(puerto_serv);
    
    // Conectar el socket del cliente con el servidor
    error = connect(socket_cliente, (struct sockaddr *) &socket_servidor,  sizeof(socket_servidor));
	if (error == -1) {
		perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al conectar el socket del cliente con el servidor\n");
		close(socket_cliente);
        return -1;
	}

    // Enviar datos al servidor
    printf("Enviando peticion.\n");
    if (enviar_peticion(socket_cliente, peticion) == -1){
        perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al enviar la peticion\n");
        close(socket_cliente);
        return -1;
    }

    // Recibir respuesta en la cola del cliente
    if (peticion->operacion == 2){      // Funcion get_value
        Peticion resp;
        printf("Recibiendo respuesta del servidor.\n");
        if (recibir_get_value(socket_cliente, &resp) == -1){
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error al recibir la respuesta\n");
            close(socket_cliente);
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
    else{   // Cualquier otra funcion
        printf("Recibiendo respuesta del servidor.\n");
        if(recvMessage(socket_cliente, (char *) &res, sizeof(int32_t)) == -1) {
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: No se puede recibir la respuesta");
            mq_close(cola_cliente);
            mq_unlink(nombre_cola);
            return -1;
        }
        respuesta = ntohl(res);
        if (respuesta == -1){
            perror("ERROR:\nFile: proxy-mq.c\nFunction: tratar_peticion\nError: Error en la respuesta");
            mq_close(cola_cliente);
            mq_unlink(nombre_cola);
            return -1;
        }
    }

    close(socket_cliente);
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