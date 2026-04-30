#!/bin/bash

source ./func.sh

# Crear sala
echo "Creando una sala con un tamaño normal."
limpia
../misala crea -f fichero -c 5
test
echo ""

# Crear sala ya existente sobreescribiendo
echo "Creando una sala previamente existente sobreescribiendo"
../misala crea -of fichero -c 5
test
echo ""

# Intentando crear sala que ya existe sin sobreescribir
echo "Creando una sala previemente existente sin sobreescribir"
../misala crea -f fichero -c 5
testFail
echo ""

# Intentando crear sala sin indicar la ruta
echo "Creando una sala sin indicar la ruta"
limpia
../misala crea -c 5
testFail
echo ""

# Intentando crear sala sin indicar la capacidad
echo "Creando una sala sin indicar la capacidad"
limpia
../misala crea -f fichero
testFail
echo ""

# Intentando crear sala con capacidad 0
echo "Creando una sala con capacidad cero"
limpia
../misala crea -f fichero -c 0
testFail
echo ""

limpia
echo "$exitos pruebas exitosas de $total pruebas realizadas"

exit $(( total - exitos ))
