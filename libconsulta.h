#ifndef LIBCONSULTA_H
#define LIBCONSULTA_H

#include <mysql/mysql.h>

// Prototipos de funciones
MYSQL *conectar_bd();
void escribir_bd(MYSQL *conn, const char *mensaje);
void cerrar_bd(MYSQL *conn);

#endif
