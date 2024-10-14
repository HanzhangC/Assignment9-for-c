#include "functions.h"

int main(int argc, char** argv) {
    int i;
    int sockfd;
    char* folder = "INBOX";
    char* password = NULL;
    char* username = NULL;
    char* msgNum = NULL;
    char* command = NULL;
    char* serverName = NULL;

    // Read input.
    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            folder = argv[i + 1];
        } else if(strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            password = argv[i + 1];
        } else if(strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            username = argv[i + 1];
        } else if(strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            msgNum = argv[i + 1];
        }
    }

    // Ensure all required arguments are provided
    if (username == NULL || password == NULL || argc < 3) {
        fprintf(stderr, "Usage: fetchmail -u <username> -p <password> [-f <folder>] [-n <messageNum>] <command> <server_name>\n");
        return 1;
    }

    command = argv[argc - 2];
    serverName = argv[argc - 1];

    sockfd = create_socket(serverName);
    if (sockfd < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }

    // read from server
    char read_buffer[BUFFER_SIZE];
    receive_msg(sockfd, read_buffer, sizeof(read_buffer));

    // write into server socket
    char write_buffer[BUFFER_SIZE];
    snprintf(write_buffer, sizeof(write_buffer), "H001 LOGIN %s %s\r\n", username, password);
    send_msg(sockfd, write_buffer);
    receive_msg(sockfd, read_buffer, sizeof(read_buffer));
    if (strstr(read_buffer, "OK") == NULL) {
        printf("Login failure\n");
        exit(3);
    }

    snprintf(write_buffer, sizeof(write_buffer), "H002 SELECT %s\r\n", folder);
    send_msg(sockfd, write_buffer);
    receive_msg(sockfd, read_buffer, sizeof(read_buffer));
    if (strstr(read_buffer, "OK") == NULL) {
        printf("Folder not found\n");
        exit(3);
    }

    if (strcmp(command, "retrieve") == 0) {
        handle_retrieve(sockfd, msgNum);
    } else if (strcmp(command, "parse") == 0) {
        handle_parse(sockfd, msgNum);
    } else if (strcmp(command, "mime") == 0) {
        handle_mime(sockfd, msgNum);
    } else if (strcmp(command, "list") == 0) {
        handle_list(sockfd, msgNum);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
    }

    return 0;
}