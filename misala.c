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
  int sel_personas = 0;

  int id;

  static struct option long_options[] = {
    {"asientos", no_argument, NULL, 'a'},
    {"personas", no_argument, NULL, 'p'},
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
      case 'p':
      	if (strcmp(argv[optind - 1], "-personas") == 0)
          sel_personas = 1;
        break;
      default:
        printf("No reconocido.\n");
        break;
    }
  }
  
  
  // misala compara ruta1 ruta2
  if (strcmp(argv[1], "compara") ==0) {
      if (argc != 4) {
      	fprintf(stderr, "Uso: misala compara ruta1 ruta2\n");
      	return -1;
      }
      
      int fd1 = open(argv[2], O_RDONLY);
      int fd2 = open(argv[3], O_RDONLY);
      
      if (fd1 == -1 || fd2 == -1) {
      	perror("Error al abrir archivos");
      	return -1;
      }
      
      int cap1, cap2, occ1, occ2;
      
      if (read(fd1, &cap1, sizeof(int)) != sizeof(int) || 
      	  read(fd2, &cap2, sizeof(int)) != sizeof(int)) {
      	perror("Error leyendo capacidad");
      	return -1;
      }
      
      if (cap1 != cap2) {
      	close(fd1);
      	close(fd2);
      	return 1;
      }
      
      if (read(fd1, &occ1, sizeof(int)) != sizeof(int) || 
      	  read(fd2, &occ2, sizeof(int)) != sizeof(int)) {
      	perror("Error leyendo ocupados");
      	return -1;
      }
      
      if (occ1 != occ2) {
      	close(fd1);
      	close(fd2);
      	return 1;
      }
      
      int buffer1[cap1];
      int buffer2[cap1];
      
      if (read(fd1, buffer1, cap1 * sizeof(int)) != cap1 * sizeof(int) ||
      	  read(fd2, buffer2, cap1 * sizeof(int)) != cap1 * sizeof(int)) {
      	perror("Error leyendo datos");
      	return -1;
      }
      
      close(fd1);
      close(fd2);
      
      for (int i =0;i < cap1; i++) {
      	if (buffer1[i] != buffer2[i]) {
      	    return 1;
      	}
      }
      
      return 0;
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
  
  // misala anula -f ruta -personas id_persona1
  if (strcmp(argv[1], "anula") == 0 && sel_personas == 1) {

    int id_asientos[capacidad_sala()];
    int num_asientos = 0;

    for (int i = optind; i < argc; i++) {
      int id_persona = atoi(argv[i]);
      int encontrado = 0;
      
      for (int j=1; j<=capacidad_sala(); j++) {
      	if (estado_asiento(j) == id_persona) {
      	    libera_asiento(j);
      	    id_asientos[num_asientos++] = j;
      	    encontrado = 1;
      	}
      }
      
      if (!encontrado) {
      	fprintf(stderr, "No se encontraron reservas para la persona %d.\n", id_persona);
      }
    }
    
    if (guarda_estado_parcial_sala(filename, num_asientos, id_asientos) == -1) {
      fprintf(stderr, "Error al guardar cambios.\n");
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
