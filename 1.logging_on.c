#include "functions.h"

// function that create socket and connect it to the server
int create_socket(char* serverName) {
    char* port = "143"; // IMAP port
    int sockfd, s;
    struct addrinfo hints, *servinfo, *rp;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    s = getaddrinfo(serverName, port, &hints, &servinfo);
    if (s != 0) { // fail to get server add
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }

    for (rp = servinfo; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) {
            continue;
        }

        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break; // Successfully connected
        }

        close(sockfd);
    }

    if (rp == NULL) { // No address succeeded
        fprintf(stderr, "client: failed to connect\n");
        freeaddrinfo(servinfo);
        return -1;
    }

    freeaddrinfo(servinfo);
    return sockfd;
}

// function to receive message from server
int receive_msg(int sockfd, char* buffer, size_t size) {
    ssize_t n = read(sockfd, buffer, size - 1);
    if (n < 0) {
        perror("Error reading from socket");
        return -1;
    }
    buffer[n] = '\0'; // Null-terminate string
    return 0;
}

// function to send message to server
int send_msg(int sockfd, char* content) { 
    ssize_t n = write(sockfd, content, strlen(content));
    if (n < 0) {
        perror("Error writing to socket");
        return -1;
    }
    return 0;
}