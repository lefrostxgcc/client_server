#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
	char buf[256] = {};
	struct sockaddr_in server_address = {};
	int client_socket;

	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8000);

	if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
	{
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	if (connect(client_socket, (struct sockaddr *) &server_address,
		sizeof(server_address)) < 0)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}
	if (read(client_socket, buf, sizeof(buf) / sizeof(buf[0])) < 0)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", buf);

	return 0; 
} 
