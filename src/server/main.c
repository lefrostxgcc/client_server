#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
	struct sockaddr_in channel;
	socklen_t channel_len;
	int server_fd;
	int opt;
	int new_socket;

	opt = 1;
	channel_len = sizeof(channel);
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = INADDR_ANY;
	channel.sin_port = htons(8000);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
					&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	if (bind(server_fd, (struct sockaddr *) &channel, sizeof(channel)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *) &channel,
		&channel_len)) < 0) 
	{ 
		perror("accept");
		exit(EXIT_FAILURE);
	}

	close(new_socket);
	close(server_fd);

	return 0; 
} 
