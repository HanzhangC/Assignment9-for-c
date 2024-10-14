#include "functions.h"

void handle_mime(int sockfd, char* msgNum) {
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    char *email = NULL;
    size_t email_len = 0;

    // Fetch the email
    snprintf(command, sizeof(command), "A01 FETCH %s BODY.PEEK[]\r\n", msgNum);
    if (write(sockfd, command, strlen(command)) < 0) {
        perror("write");
        exit(1);
    }

    // Read the response
    ssize_t bytes_read;
    while ((bytes_read = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        email_len += bytes_read;
        email = realloc(email, email_len + 1);
        if (!email) {
            perror("realloc");
            exit(1);
        }
        if (email_len == bytes_read) {
            strcpy(email, buffer);
        } else {
            strcat(email, buffer);
        }
        if (strstr(buffer, "\r\nA01 OK") != NULL) {
            break;
        }
    }
    if (bytes_read < 0) {
        perror("read");
        exit(1);
    }

    // MIME parsing
    char boundary[BUFFER_SIZE / 4] = "";
    char *line = strtok(email, "\r\n");

    // Extract boundary
    while (line != NULL) {
        if (strncmp(line, " boundary=", 10) == 0) {
            // Get the boundary value
            char *boundary_start = line + 10;
            if (*boundary_start == '"') {
                boundary_start++;
            }
            char *boundary_end = strchr(boundary_start, '"');
            if (boundary_end != NULL) {
                *boundary_end = '\0';
            }
            strncpy(boundary, boundary_start, sizeof(boundary) - 1);
            boundary[sizeof(boundary) - 1] = '\0';
            break;
        }
        line = strtok(NULL, "\r\n");
    }

    if (strlen(boundary) == 0) {
        fprintf(stderr, "Boundary not found\n");
        free(email);
        exit(4);
    }

    // Prepare the boundary delimiter
    char boundary_delim[BUFFER_SIZE / 4 + 3];
    snprintf(boundary_delim, sizeof(boundary_delim), "--%s", boundary);

    // Skip lines until the first boundary
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        if (strcmp(line, boundary_delim) == 0) {
            break;
        }
    }

    // Traverse the email and extract content
    int print_content = 0;
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        if (strcmp(line, boundary_delim) == 0) {
            // End of content
            break;
        } else {
            if (print_content) {
                if (strncmp(line, "Content-Type:", 13) != 0 && strncmp(line, " charset=UTF-8", 14) != 0 && strncmp(line, "Content-Transfer-Encoding:", 26) != 0) {
                    printf("%s\n", line);
                } else {
                    printf("\n\n");
                }
            } else if (strlen(line) > 0) {
                print_content = 1;
            }
        }
    }

    free(email);
}