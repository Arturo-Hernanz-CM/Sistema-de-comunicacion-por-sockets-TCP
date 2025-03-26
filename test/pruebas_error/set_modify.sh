# Prueba set y modify de dos keys diferentes

#!/bin/bash

# Función para limpiar
cleanup(){
    echo "Limpiando..."
    pkill -P $$
    exit 1
}

trap cleanup SIGINT

./app-cliente-6
./app-cliente-3


wait 
exit 0