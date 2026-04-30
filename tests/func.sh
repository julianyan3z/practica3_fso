#!/bin/bash

test () {
  if [ $(echo $?) -eq 0 ]; then
    echo "Prueba exitosa"
    exitos=$((exitos + 1))
  else
    echo "Prueba fallida"
  fi
  total=$((total + 1))
}

testFail () {
  if [ $(echo $?) != 0 ]; then
    echo "Prueba exitosa"
    exitos=$((exitos + 1))
  else
    echo "Prueba fallida"
  fi
  total=$((total + 1))
}

limpia () {
  if [ -e fichero ]; then
    rm fichero
  fi
}

preparar_sala () {
  limpia
  ../misala crea -f fichero -c 5
  if [ $(echo $?) != 0 ]; then
    echo "Ha ocurrido un error en la prueba"
    exit
  fi
}
