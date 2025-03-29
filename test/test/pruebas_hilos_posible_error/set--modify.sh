# Prueba set y modify al mismo tiempo 

#!/bin/bash

# Función para limpiar
cleanup(){
    echo "Limpiando..."
    pkill -P $$
    exit 1
}

trap cleanup SIGINT

./app-cliente-1&
./app-cliente-3


wait 
exit 0