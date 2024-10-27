#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <json-c/json.h>
#include <pwd.h>

void usage(const char *program)
{
	printf("Usage: %s -c \"command\" -h \"host\" -p port [-s | -d] [--help]\n", program);
}

int main(int argc, char *argv[])
{
	int opt;
	const char *command = NULL;
	const char *host = NULL;
	int port = 0;
	int use_stream = 0;
	int use_dgram = 0;

	while ((opt = getopt(argc, argv, "c:h:p:sd")) != -1)
	{
		switch (opt)
		{
		case 'c':
			command = optarg;
			break;
		case 'h':
			host = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 's':
			use_stream = 1;
			break;
		case 'd':
			use_dgram = 1;
			break;
		case '?':
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (!command || !host || port == 0)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_addr;
	int sockfd;

	if (use_stream)
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
	}
	else if (use_dgram)
	{
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	}
	else
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, host, &server_addr.sin_addr);

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}

	struct passwd *pw = getpwuid(getuid());
	char *username = pw->pw_name;

	char *json_request = json_object_to_json_string_ext(
		json_object_new_string(
			json_object_new_stringf("{\"login\":\"%s\",\"command\":\"%s\"}", username, command)),
		JSON_C_TO_STRING_PRETTY);

	if (send(sockfd, json_request, strlen(json_request), 0) == -1)
	{
		perror("send");
		exit(EXIT_FAILURE);
	}

	char buffer[1024];
	if (recv(sockfd, buffer, sizeof(buffer), 0) == -1)
	{
		perror("recv");
		exit(EXIT_FAILURE);
	}

	printf("Server response: %s\n", buffer);

	close(sockfd);
	return 0;
}
