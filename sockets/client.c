#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>



const char* SERVER_IP = "127.0.0.1";

int main(int argc, char* argv[]) {

	if (argc < 2) {
		printf("ERROR: Se debe enviar un mensaje por parametro.\n");
		exit(-1);
	}

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		perror("Error en socket");
		exit(-1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);

	struct hostent* server = gethostbyname (SERVER_IP) ;
	if (server == NULL) {
		perror("Error en gethostbyname");
		exit(-1);
	}

	bcopy (server->h_addr,
	       &(server_addr.sin_addr.s_addr),
	       server->h_length);

	if (connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))) {
		perror("Error en connect");
		exit(-1);
	}

	printf("Conexion exitosa.\n");
	char* msg = argv[1];

	printf("Enviando: %s\n", msg);
	if (write(socket_fd, msg, strlen(msg)) < 0) {
		perror("Error en write.");
	} else {
		if (read(socket_fd, msg, strlen(msg)) < 0) {
			perror("Error en read");
		} else {
			printf("Llego: %s\n", msg);
		}
	}

	printf("Cerrando socket.\n");
	if (close(socket_fd) < 0) {
		perror("Error en close");
	}

	printf("Fin cliente\n");
}