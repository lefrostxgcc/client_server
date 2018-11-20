#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

static int create_client_socket(const char *address, int port);

int main(int argc, char *argv[]) 
{
	char		buf[256] = {};
	int			client_socket;

	client_socket = create_client_socket("127.0.0.1", 8000);

	if (read(client_socket, buf, sizeof(buf) / sizeof(buf[0])) < 0)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", buf);
	close(client_socket);

	return 0; 
}

static int create_client_socket(const char *address, int port)
{
	struct sockaddr_in	server_address = {};
	int					client_socket;

	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	if (inet_pton(AF_INET, address, &server_address.sin_addr) <= 0)
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

	return client_socket;
}
