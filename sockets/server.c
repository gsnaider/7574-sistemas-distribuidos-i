#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char* argv[]) {

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8080);
	myaddr.sin_addr.s_addr = INADDR_ANY;


	socklen_t myaddr_size = sizeof(myaddr);

	if (bind(socket_fd, (struct sockaddr*) &myaddr, myaddr_size ) < 0) {
		perror("Error en bind");
		exit(-1);
	}

	if (listen(socket_fd, 10) < 0) {
		perror("Error en listen");
		exit(-1);
	}

	struct sockaddr_in client_addr;

	socklen_t client_addr_size = sizeof(client_addr);


	printf("Esperando conexiones...\n");
	int client_fd = accept(socket_fd, (struct sockaddr*) &client_addr, &client_addr_size);

	if (client_fd < 0) {
		perror("Error en accept");
		exit(-1);
	}
	char buffer[100];
	while (read(client_fd, buffer, sizeof(buffer) / sizeof(char)) >= 0) {
		printf("Llego %s\n", buffer);
		if (write(client_fd, buffer, sizeof(buffer) / sizeof(char)) < 0) {
			perror("Error en write");
			exit(-1);
		}
		printf("LLEGO!\n");
	}
	printf("Fin server\n");
}