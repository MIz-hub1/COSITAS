#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <unistd.h>

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "universidad/alumnos"

// Estructura del mensaje
typedef struct {
    char nombre[50];
    char apellido1[50];
    char apellido2[50];
    char matricula[20];
    char carrera[100];
    int num_materias;
    char materias[10][100];
    float calificaciones[10];
} Alumno;

void publicar(struct mosquitto *mosq, Alumno alumno) {
    char mensaje[1024];
    snprintf(mensaje, sizeof(mensaje), "%s,%s,%s,%s,%s,%d",
             alumno.nombre, alumno.apellido1, alumno.apellido2,
             alumno.matricula, alumno.carrera, alumno.num_materias);

  for (int i = 0; i < alumno.num_materias; i++) {
    strcat(mensaje, (i == 0) ? "" : ";");  // Separar materias con ;
    strcat(mensaje, alumno.materias[i]);

    snprintf(mensaje + strlen(mensaje), sizeof(mensaje) - strlen(mensaje), ";%.2f",
             alumno.calificaciones[i]);
}

    mosquitto_publish(mosq, NULL, TOPIC, strlen(mensaje), mensaje, 0, false);
    printf("Mensaje publicado: %s\n", mensaje);
}

int main(int argc, char *argv[]) {
    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);

    if (!mosq) {
        fprintf(stderr, "Error al crear el cliente mosquitto\n");
        return 1;
    }

    if (mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker\n");
        return 1;
    }

    // Lista de alumnos
    Alumno alumnos[] = {
        {"Juanita", "Zalit", "Opez", "Z12345", "Ingenieria en Administracion", 7, "Calculo", "Estructura", 7.5, 9.8},
        {"Maria", "Gonzalez", "Sanchez", "B67890", "Informatica", 3, "Redes", "Seguridad", "Base de Datos", 8.7, 9.2, 7.8},
        {"Carlos", "Ruiz", "Gomez", "C54321", "Sistemas", 1, "Analisis de Algoritmos", 8.0}
    };

    int total_alumnos = sizeof(alumnos) / sizeof(Alumno);

    // Publicar datos de cada alumno
    for (int i = 0; i < total_alumnos; i++) {
        publicar(mosq, alumnos[i]);
        sleep(1);  // Pequeño retraso entre envíos
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
