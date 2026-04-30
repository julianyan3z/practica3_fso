#!/bin/bash

source ./func.sh

# Reservar un asiento
echo "Reservando un asiento"
preparar_sala
../misala reserva -f fichero 1
test
echo ""

# Reservar dos asientos
echo "Reservando dos asientos"
../misala reserva -f fichero 2 3
test
echo ""

# Reservar un id inválido
echo "Intentando reservar un id inválido"
preparar_sala
../misala reserva -f fichero 0
testFail
echo ""

# Reservar tantos asientos como capacidad tenga la sala
echo "Reservando tantos asientos como capacidad tenga la sala"
preparar_sala
../misala reserva -f fichero 1 2 3 4 5
test
echo ""

# Reservar un asiento más que la capacidad de la sala
echo "Reservando un asiento más que la capacidad de la sala"
preparar_sala
../misala reserva -f fichero 1 2 3 4 5 6
testFail
echo ""

# Reservar después de haber reservado toda la sala
echo "Reservando un asiento después de haber reservado toda la sala"
preparar_sala
../misala reserva -f fichero 1 2 3 4 5
../misala reserva -f fichero 6
testFail
echo ""

limpia
echo "$exitos pruebas exitosas de $total pruebas realizadas"

exit $(( total - exitos ))
