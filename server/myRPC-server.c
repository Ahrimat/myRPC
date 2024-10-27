
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <json-c/json.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <libgen.h>
#include <libmysyslog/libmysyslog.h>

void handle_client(int client_sock)
{
	char buffer[1024];
	if (recv(client_sock, buffer, sizeof(buffer), 0) == -1)
	{
		perror("recv");
		close(client_sock);
		return;
	}

	struct json_object *parsed_json = json_tokener_parse(buffer);
	const char *username = json_object_object_get_string(parsed_json, "login");
	const char *command = json_object_object_get_string(parsed_json, "command");

	FILE *fp = fopen("/etc/myRPC/users.conf", "r");
	char allowed_user[256];
	while (fgets(allowed_user, sizeof(allowed_user), fp))
	{
		allowed_user[strcspn(allowed_user, "\n")] = 0;
		if (strcmp(username, allowed_user) == 0)
		{
			fclose(fp);
			break;
		}
	}
	fclose(fp);

	if (strcmp(username, allowed_user) != 0)
	{
		char *json_response = json_object_to_json_string_ext(
			json_object_new_string("{\"code\":1,\"result\":\"Access denied\"}"),
			JSON_C_TO_STRING_PRETTY);
		send(client_sock, json_response, strlen(json_response), 0);
		free(json_response);
		close(client_sock);
		return;
	}

	int pipefd[2];
	pipe(pipefd);

	pid_t pid = fork();
	if (pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		char *args[] = {"sh", "-c", command, NULL};
		execvp(args[0], args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipefd[0]);
		char result[1024];
		read(pipefd[1], result, sizeof(result));
		close(pipefd[1]);

		char *json_response = json_object_to_json_string_ext(
			json_object_new_stringf("{\"code\":0,\"result\":\"%s\"}", result),
			JSON_C_TO_STRING_PRETTY);
		send(client_sock, json_response, strlen(json_response), 0);
		free(json_response);
	}

	close(client_sock);
}

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	int opt = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(1234);

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 5) == -1)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client_sock == -1)
		{
			perror("accept");
			continue;
		}

		handle_client(client_sock);
	}

	close(sockfd);
	return 0;
}
