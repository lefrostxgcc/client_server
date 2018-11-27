#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
	const char header_fmt[] = 
		"HTTP/1.x 200 OK\r\n"
		"Cache-Control: no-cache\r\n"
		"Connection: close\r\n"
		"Server: videosharp\r\n"
		"Content-Length: %d\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n"
		"\r\n";

	const char msg_fmt[] = "<h1>Java %d</h1>\r\n";

	char buf[sizeof(header_fmt) + sizeof(msg_fmt) + 40];
	char header_buf[sizeof(header_fmt) + 20];
	char msg_buf[sizeof(msg_fmt) + 20];
	struct sockaddr_in channel;
	int server_fd;
	int client_socket;
	int opt;
	int count;
	int header_len;
	int content_len;
	int responce_len;

	opt = 1;
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
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

	printf("Server started\n");
	count = 0;
	while (1)
	{
		client_socket = accept(server_fd, 0, 0);
		if (client_socket < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		count++;
		printf("Client accepted %d\n", count);
		content_len = snprintf(msg_buf, sizeof(msg_buf), msg_fmt, count);
		header_len = snprintf(header_buf, sizeof(header_buf),
			header_fmt, content_len);
		responce_len = snprintf(buf, sizeof(buf), "%s%s", header_buf, msg_buf);
		write(client_socket, buf, responce_len);
		close(client_socket);
	}

	return 0; 
} 
