#!/bin/bash

source ./func.sh


# Estado de una sala existente
echo "Comprobando el estado de una sala que existe"
preparar_sala
../misala estado -f fichero
test
limpia
echo ""

# Estado de una sala que no existe
echo "Comprobando el estado de una sala que no existe"
../misala estado -f sala_no_existente
testFail
echo ""


echo "$exitos pruebas exitosas de $total pruebas realizadas"

exit $(( total - exitos ))

