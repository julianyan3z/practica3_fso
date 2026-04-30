#ifndef SALA
#define SALA

// include para el 'size_t'
#include <stdlib.h>

int reserva_asiento(int id_persona);

int libera_asiento(int id_asiento);

int estado_asiento(int id_asiento);

int asientos_libres();

int asientos_ocupados();

int capacidad_sala();

int crea_sala(int capacidad);

int elimina_sala();

// Funciones para guardar y cargar el estado de la sala en ficheros

int guarda_estado_sala(const char* ruta_fichero);

int recupera_estado_sala(const char* ruta_fichero);

int guarda_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos);

int recupera_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos);

#endif
