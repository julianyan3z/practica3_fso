#!/bin/bash

source ./func.sh

echo "Anulando por personas"

preparar_sala
../misala reserva -f fichero 1 2 3 > /dev/null

../misala anula -f fichero -personas 2
res=$?

if [ $res -eq 0 ]; then
	echo "Anulación ejecutada correctamente"
	../misala estado -f fichero | grep "persona 2" > /dev/null
	if [ $? -ne 0 ]; then
		echo "Verificación: la persona 2 ya no está en la sala"
		test
	else
		echo "Error: la persona 2 sigue en la sala"
		testFail
	fi
else
	testFail
fi
echo""

echo "Anulando persona inexistente"
../misala anula -f fichero -personas 99
res=$?

if [ $res -eq 0 ]; then
	echo "Comportamiento correcto ante la persona inexistente"
	test
else
	testFail
fi

echo""

limpia
echo "$exitos pruebas exitosas de $total pruebas realizadas"

exit $(( total - exitos ))
