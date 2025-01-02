
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "12345"
#define DB_NAME "universidad"

MYSQL *conectar_bd() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() fallo\n");
        return NULL;
    }

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error de conexion: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    printf("Conexion exitosa a la base de datos.\n");
    return conn;
}

void escribir_bd(MYSQL *conn, const char *mensaje) {
    char query[2048];
    char nombre[50], apellido1[50], apellido2[50], matricula[20], carrera[100];
    int num_materias;
    char materias[512] = {0};
    char calificaciones[512] = {0};
    
    printf("Mensaje recibido para inserción: %s\n", mensaje);

    // Extraer los datos del mensaje (CSV)
    int parsed = sscanf(mensaje, "%49[^,],%49[^,],%49[^,],%19[^,],%99[^,],%d,%511[^,],%511[^\n]",
                        nombre, apellido1, apellido2, matricula, carrera, &num_materias, materias, calificaciones);

if (parsed < 6) {
        fprintf(stderr, "Error: Formato del mensaje incorrecto. Solo se parsearon %d campos.\n", parsed);
        return;
    }

    printf("Datos parseados:\nNombre: %s\nApellido1: %s\nApellido2: %s\nMatricula: %s\nCarrera: %s\nMaterias: %s\nCalificaciones: %s\n",
           nombre, apellido1, apellido2, matricula, carrera, materias, calificaciones);

    // Crear la consulta para insertar en la tabla alumnos
    snprintf(query, sizeof(query),
             "INSERT INTO alumnos (nombre, apellido1, apellido2, matricula, carrera, num_materias, materias, calificaciones) "
             "VALUES ('%s', '%s', '%s', '%s', '%s', %d, '%s', '%s')",
             nombre, apellido1, apellido2, matricula, carrera, num_materias, materias, calificaciones);

printf("Ejecutando query: %s\n", query);

    // Ejecutar la consulta
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al insertar datos: %s\n", mysql_error(conn));
    } else {
        printf("Datos insertados correctamente.\n");
    }
}
