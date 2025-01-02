#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <mysql/mysql.h>
#include "libconsulta.h"
#include <unistd.h>

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "universidad/alumnos"

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
    printf("Conectado al broker con codigo %d\n", rc);
    mosquitto_subscribe(mosq, NULL, TOPIC, 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    printf("Mensaje recibido: %s\n", (char *)msg->payload);

    MYSQL *conn = conectar_bd();
    if (conn) {
        escribir_bd(conn, (char *)msg->payload);
        mysql_close(conn);
    } else {
        fprintf(stderr, "Error de conexion a la base de datos\n");
    }
}

int main(int argc, char *argv[]) {
    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);

    if (!mosq) {
        fprintf(stderr, "Error al crear el cliente mosquitto\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker\n");
        return 1;
    }

    mosquitto_loop_start(mosq);
    printf("Esperando mensajes... Presione Ctrl+C para salir.\n");
    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
