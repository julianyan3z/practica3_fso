#!/bin/bash

gcc -o ../misala ../misala.c ../lib/sala.c

if [ $(echo $?) != 0 ]; then
  echo "Ha habido un problema en la compilación"
  exit
fi

fallos=0

echo "Ejecutando tests para misala crea"
echo ""
./crea.sh
fallos=$(( fallos + $(echo $?)))
echo "-------------------------------------------"
echo ""

echo "Ejecutando tests para misala reserva"
echo ""
./reserva.sh
fallos=$(( fallos + $(echo $?)))
echo "-------------------------------------------"
echo ""

echo "Ejecutando tests para misala anula asientos"
echo ""
./anula_asientos.sh
fallos=$(( fallos + $(echo $?)))
echo "-------------------------------------------"
echo ""


echo "Ejecutando tests para misala estado"
echo ""
./estado.sh
fallos=$(( fallos + $(echo $?)))
echo "-------------------------------------------"

echo "Ejecutando tests para misala personas"
echo ""
./anula_personas.sh
fallos=$(( fallos + $(echo $?)))
echo "-------------------------------------------"

echo "Ejecutando tests para misala compara"
echo ""
./compara.sh
fallos=$(( fallos + $(echo $?)))
echo "-------------------------------------------"

echo ""
echo "Han ocurrido $fallos fallos"
