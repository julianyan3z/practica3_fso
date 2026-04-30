#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include "lib/sala.h"


int main(int argc, char** argv) {

  if (argc < 4) {
    fprintf(stderr, "Faltan argumentos.\n");
    return -1;
  }

  // Declaración de variables
  extern char *optarg;
  extern int optind;
  optind = 2;
  
  int file;
  char* filename = NULL;

  int capacity;
  int overwrite = 0;
  int sel_asientos = 0;

  int id;

  static struct option long_options[] = {
    {"asientos", no_argument, NULL, 'a'},
    {NULL, 0, NULL, 0}
  };
  
  // Obtención de parámetros
  char c;
  while ((c = getopt_long_only(argc, argv, "f:oc:", long_options, NULL)) != -1)  {
    switch (c) {
      case 'f':
        filename = optarg;
        break;
      case 'o':
        overwrite = 1;
        break;
      case 'c':
        capacity = atoi(optarg);
        break;
      case 'a':
        if (strcmp(argv[optind - 1], "-asientos") == 0)
          sel_asientos = 1;
        break;
      default:
        printf("No reconocido.\n");
        break;
    }
  }


  // Comprobamos si se ha especificado ruta
  if (filename == NULL) {
    fprintf(stderr, "No se ha indicado fichero.\n");
    return -1;
  }

  // misala crea -f[o] ruta -c capacidad
  if (strcmp(argv[1], "crea") == 0) {
    // Error si la capacidad no es válida
    if (capacity <= 0) {
      fprintf(stderr, "Introduzca un valor de capacidad correcto.\n");
      return -1;
    }

    // Error si el fichero existe y no se ha indicado sobreescritura
    if (access(filename, F_OK) == 0 && !overwrite) {
      fprintf(stderr, "El fichero ya existe y no se ha especificado sobreescritura.\n");
      return -1;
    }

    // Error si no se ha podido crear la sala o esta no se ha podido guardar en el fichero
    if (crea_sala(capacity) == -1 || guarda_estado_sala(filename) == -1) {
      fprintf(stderr, "Ha ocurrido un error al intentar guardar la sala.\n");
      return -1;
    }
    
    return 0;
  }

  // Comprobación de la existencia de la sala y obtención de su capacidad
  file = open(filename, O_RDONLY, 0);
  if (file == -1) {
    perror("Ha ocurrido un error respecto al archivo de guardado");
    return -1;
  }

  // Leemos el primer byte del archivo para obtener la capacidad de la sala y comprobamos que sea válido
  if (read(file, &capacity, sizeof(int)) != sizeof(int) || capacity <= 0) {
    perror("No se ha podido obtener la capacidad de la sala");
    return -1;
  }

  close(file);

  if (crea_sala(capacity) == -1 || recupera_estado_sala(filename) == -1) {
    fprintf(stderr, "Error al cargar la sala");
    return -1;
  }

  // misala reserva -f ruta id_persona1 id_persona2 ...
  if (strcmp(argv[1], "reserva") == 0) {

    int id_personas[argc-4];
    int num_personas = 0;
    
    for (int i = optind; i < argc; i++) {
      // Si al intentar reservar un asiento no queda espacio en la sala, no se realizan cambios en la sala
      if (asientos_libres() == 0) {
        fprintf(stderr, "No quedan asientos en la sala para reservar.\n");
        return -1;
      }
      id = atoi(argv[i]);
      if (id <= 0) {
        fprintf(stderr, "El id de la persona es inválido.\n");
        return -1;
      }
      id_personas[num_personas] = reserva_asiento(id);
      if (id_personas[num_personas] == -1) {
        fprintf(stderr, "Ha ocurrido un error al reservar asientos.\n");
        return -1;
      }
      printf("La persona con id %d ha sido asignado al asiento %d\n", id, id_personas[num_personas]);
      num_personas++;
    }

    if (guarda_estado_parcial_sala(filename, num_personas, id_personas) == -1) {
      fprintf(stderr, "Ha ocurrido un error al guardar el estado de nuevo en el fichero.\n");
      return -1;
    }

    return 0;
  }

  // misala anula -f ruta -asientos id_asiento1 [id_asiento2 ...]
  if (strcmp(argv[1], "anula") == 0 && sel_asientos == 1) {

    int id_asientos[argc-5];
    int num_asientos = 0;

    for (int i = optind; i < argc; i++) {
      id = atoi(argv[i]);
      // Si el id del asiento a reservar es inválido, este no se tiene en cuenta
      if (id <= 0 || id > capacidad_sala()) {
        fprintf(stderr, "El id %d es inválido.\n", id);
        continue;
      }
      if (libera_asiento(id) != -1) {
        id_asientos[num_asientos] = id;
        num_asientos++;
      }
    }

    if (guarda_estado_parcial_sala(filename, num_asientos, id_asientos) == -1) {
      fprintf(stderr, "Ha ocurrido un error al guardar el estado de nuevo en el fichero.\n");
      return -1;
    }

    return 0;
  }  

  // misala estado -f ruta
  if (strcmp(argv[1], "estado") == 0) {
    printf("Hay %d asientos libres.\n", asientos_libres());
    for (int i = 1; i <= capacidad_sala(); i++) {
      if ((id = estado_asiento(i)) == 0) {
        printf("El asiento %d está vacío.\n", i);
      } else {
        printf("El asiento %d está ocupado por la persona %d.\n", i, id);
      }
    }

    return 0;
  }

  return 0;
}
