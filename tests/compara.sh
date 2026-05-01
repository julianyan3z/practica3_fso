#!/bin/bash

source ./func.sh

echo "Comparando salas iguales"

limpia
../misala crea -f sala1 -c 5
../misala crea -f sala2 -c 5

../misala compara sala1 sala2
res=$?

if [ $res -eq 0 ]; then
	echo "Las salas son iguales"
	test
else
	testFail
fi

echo ""

echo "Comparando salas diferentes"

../misala reserva -f sala1 1 > /dev/null

../misala compara sala1 sala2
res=$?

if [ $res -ne 0 ]; then
	echo "Las salas son diferentes"
	test
else
	testFail
fi

echo ""

rm -f sala1 sala2

echo "$exitos pruebas exitosas de $total pruebas realizadas"

exit $(( total - exitos ))
