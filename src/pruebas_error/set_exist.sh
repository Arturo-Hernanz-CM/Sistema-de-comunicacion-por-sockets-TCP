# Prueba set y exist de dos keys diferentes

#!/bin/bash

# Función para limpiar
cleanup(){
    echo "Limpiando..."
    pkill -P $$
    exit 1
}

trap cleanup SIGINT

env IP_TUPLAS=localhost PORT_TUPLAS=5850 ./app-cliente-6 
env IP_TUPLAS=localhost PORT_TUPLAS=5850 ./app-cliente-5


wait 
exit 0