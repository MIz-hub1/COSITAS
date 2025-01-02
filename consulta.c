#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

// Función para conectar a la base de datos
void conectar_db(MYSQL *conn) {
    mysql_init(conn);
    if (mysql_real_connect(conn, "localhost", "usuario", "password", "base_datos", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error de conexión: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

// Función para escribir datos en la base de datos
void escribir_db(MYSQL *conn, const char *mensaje) {
    char query[1024];
    snprintf(query, sizeof(query), "INSERT INTO estudiantes (datos) VALUES ('%s')", mensaje);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}
