#!/bin/bash

source ./func.sh


# Anular un asiento válido
echo "Anulando un asiento válido"
preparar_sala
../misala reserva -f fichero 1 2 3
../misala anula -f fichero -asientos 1
test
echo ""

# Anular un asiento inválido
echo "Anulando un asiento inválido"
../misala anula -f fichero -asientos 20
test
echo ""

echo "Anulando un asiento inválido (0)"
../misala anula -f fichero -asientos 0
test
echo ""

# Anular un asiento libre
echo "Anulando un asiento libre"
../misala anula -f fichero -asientos 1
test
echo ""

limpia
echo "$exitos pruebas exitosas de $total pruebas realizadas"

exit $(( total - exitos ))
