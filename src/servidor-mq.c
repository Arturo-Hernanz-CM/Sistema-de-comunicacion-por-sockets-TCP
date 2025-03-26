#include "claves.h"
#include "estructuras.h"
#include <stdio.h> 
#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// Variables globales
Lista * base_datos = NULL;
mqd_t  cola_servidor;
pthread_mutex_t mutex_mensaje;
pthread_cond_t condicion_mensaje;
pthread_mutex_t mutex_funcion;
pthread_cond_t condicion_funcion;
int mensaje_copiado = 0;


void leer_cola(Peticion * peticion) {
    if(peticion == NULL) {
        perror("ERROR:\nFile: servidor-mq.c\nFunction: leer_cola\nError: Peticion no valida");
        return;
    }
    mqd_t cola_cliente;
    Peticion peticion_local = {0};
    Peticion resp = {0};
    int respuesta;

    // Copiar la petición a local
    pthread_mutex_lock(&mutex_mensaje);
    memcpy(&peticion_local, peticion, sizeof(Peticion));
    mensaje_copiado = 1;
    pthread_cond_signal(&condicion_mensaje);
    pthread_mutex_unlock(&mutex_mensaje);

    // Ejecutar la petición
    if (peticion_local.operacion == 0) {
        pthread_mutex_lock(&mutex_funcion);
        respuesta = destroy();
        pthread_mutex_unlock(&mutex_funcion);
    }
    else if (peticion_local.operacion == 1) {
        pthread_mutex_lock(&mutex_funcion);
        respuesta = set_value(peticion_local.key, peticion_local.value1, peticion_local.N_value2, peticion_local.value2, peticion_local.value3);
        pthread_mutex_unlock(&mutex_funcion);
    }
    else if (peticion_local.operacion == 2) {
        pthread_mutex_lock(&mutex_funcion);
        resp.key = peticion_local.key;
        respuesta = get_value(resp.key, resp.value1, &resp.N_value2, resp.value2, &resp.value3);
        pthread_mutex_unlock(&mutex_funcion);
    }
    else if (peticion_local.operacion == 3) {
        pthread_mutex_lock(&mutex_funcion);
        respuesta = modify_value(peticion_local.key, peticion_local.value1, peticion_local.N_value2, peticion_local.value2, peticion_local.value3);
        pthread_mutex_unlock(&mutex_funcion);
    }
    else if (peticion_local.operacion == 4) {
        pthread_mutex_lock(&mutex_funcion);
        respuesta = delete_key(peticion_local.key);
        pthread_mutex_unlock(&mutex_funcion);
    }
    else if (peticion_local.operacion == 5) {
        pthread_mutex_lock(&mutex_funcion);
        respuesta = exist(peticion_local.key);
        pthread_mutex_unlock(&mutex_funcion);
    }
    else {
        respuesta = -1;
    }

    // Enviar la respuesta
    printf("Enviando respuesta a: %s\n", peticion_local.cola_cliente);
    cola_cliente = mq_open(peticion_local.cola_cliente, O_WRONLY);
    if (cola_cliente == -1) {
        perror("ERROR:\nFile: servidor-mq.c\nFunction: leer_cola\nError: No se puede abrir la cola del cliente");
		mq_close(cola_cliente);
		mq_unlink(peticion_local.cola_cliente);
    }
    else {
        if(peticion_local.operacion == 2) {
            if (respuesta == -1) {
                resp = peticion_local;
                resp.N_value2 = -1;
            }
            if (mq_send(cola_cliente, (char *)&resp, sizeof(Peticion), 0) == -1) {
                perror("ERROR:\nFile: servidor-mq.c\nFunction: leer_cola\nError: No se puede enviar la respuesta");
                mq_close(cola_servidor);
                mq_unlink("/Servidor-100495850");
            }  
        }
        else{
          if (mq_send(cola_cliente, (char *)&respuesta, sizeof(respuesta), 0) == -1) {
                perror("ERROR:\nFile: servidor-mq.c\nFunction: leer_cola\nError: No se puede enviar la respuesta");
                mq_close(cola_servidor);
                mq_unlink("/Servidor-100495850");
            }  
        }
        
    }
}

int main(void) {
    // Inicializar la lista de claves
    base_datos = (Lista *) malloc(sizeof(Lista));
    if (base_datos == NULL) {
        perror("ERROR:\nFile: servidor-mq.c\nFunction: main\nError: Error al reservar memoria para la lista de claves");
        return -1;
    }
    base_datos->inicio = NULL;
    base_datos->final = NULL;
    base_datos->size = 0;

    // Inicializar estructuras
    Peticion peticion;
    struct mq_attr atributos_cola_s;

    // Inicializar hilos
    pthread_t thread;
    pthread_attr_t thread_att;
    pthread_attr_init(&thread_att);


    // Inicializar mutex
    pthread_mutex_init(&mutex_mensaje, NULL);
    pthread_cond_init(&condicion_mensaje, NULL);

    pthread_mutex_init(&mutex_funcion, NULL);
    pthread_cond_init(&condicion_funcion, NULL);

    // Inicializar la cola del serviddor. 
    //El servidor solo lee peticiones aquí, las respuestas en cada cola de cada cliente.
    atributos_cola_s.mq_flags = 0;         
    atributos_cola_s.mq_maxmsg = 10;
    atributos_cola_s.mq_msgsize = sizeof(Peticion);
    cola_servidor = mq_open("/Servidor-100495850", O_CREAT | O_RDONLY, 0777, &atributos_cola_s);
    if (cola_servidor == -1) {
        perror("ERROR:\nFile: servidor-mq.c\nFunction: main\nError: Error al abrir la cola del servidor");
        return -1;
    }

    // Escribir en la cola
	pthread_attr_setdetachstate(&thread_att, PTHREAD_CREATE_DETACHED);      // threads independientes
    while(1) {
        if (mq_receive(cola_servidor, (char *) &peticion, sizeof(Peticion), 0) < 0 ){
			perror("ERROR:\nFile: servidor-mq.c\nFunction: main\nError: Error al recibir la peticion");
			return -1;
		}
        printf("\nRecibiendo peticion del cliente.\n");
        if(pthread_create(&thread, &thread_att, (void*) leer_cola, &peticion) == 0) {
            pthread_mutex_lock(&mutex_mensaje);
            while(mensaje_copiado == 0) {
                pthread_cond_wait(&condicion_mensaje, &mutex_mensaje);
            }
            mensaje_copiado = 0;
            pthread_mutex_unlock(&mutex_mensaje);
        }
    }

    // Se "cierra" la cola del servidor
    mq_close(cola_servidor);
    mq_unlink("/Servidor-100495850");
    return 0;
}