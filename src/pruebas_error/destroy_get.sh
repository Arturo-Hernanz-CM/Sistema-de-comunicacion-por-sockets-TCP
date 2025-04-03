# Prueba destroy y get de dos keys diferentes

#!/bin/bash

# Función para limpiar
cleanup(){
    echo "Limpiando..."
    pkill -P $$
    exit 1
}

trap cleanup SIGINT

env IP_TUPLAS=localhost PORT_TUPLAS=5850 ./app-cliente-0
env IP_TUPLAS=localhost PORT_TUPLAS=5850 ./app-cliente-2


wait 
exit 0