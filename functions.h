#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define _POSIX_C_SOURCE 200112L
#define BUFFER_SIZE 8192


int create_socket(char* serverName);
int receive_msg(int sockfd, char* buffer, size_t size);
int send_msg(int sockfd, char* content);
void handle_retrieve(int sockfd, char* msgNum);
void handle_parse(int sockfd, char* msgNum);
char *strcasestr_custom(const char *haystack, const char *needle);
void handle_mime(int sockfd, char* msgNum);
void handle_list(int sockfd, char* msgNum);
void process_buffer(const char *read_buffer);

#endif // FUNCTIONS_H