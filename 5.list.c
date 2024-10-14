#include "functions.h"

// Fetch each seqnum seperately
void handle_list(int sockfd, char* msgNum) {
    char write_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];

    // Extracted with fetch based on serial number
    snprintf(write_buffer, BUFFER_SIZE, "H006 SEARCH ALL\r\n");
    ssize_t n = send_msg(sockfd, write_buffer);

    if(n < 0) {
        printf("Write error!\n");
        exit(EXIT_FAILURE);
    }

    n = receive_msg(sockfd, read_buffer, BUFFER_SIZE);
    
    // Extract seqnum from read buffer
    size_t i;
    for(i = 0; i < strlen(read_buffer) - 1; i++) {
        if(read_buffer[i] == '\r' && read_buffer[i + 1] == '\n') {
            break;
        }
    }
    read_buffer[i] = '\0'; // readbuffer：‘ * search 1 2 3’

    char* delim = " ";
    // get '*'
    char* token = strtok(read_buffer, delim); // split read_buffer by space
    // get 'search'
    token = strtok(NULL, delim);

    // get '1 2 3'
    while((token = strtok(NULL, delim)) != NULL) {
        snprintf(write_buffer, BUFFER_SIZE, "H007 FETCH %s BODY.PEEK[HEADER.FIELDS (SUBJECT)]\r\n", token);
        send_msg(sockfd, write_buffer);
        receive_msg(sockfd, read_buffer, BUFFER_SIZE);
        
        // Check if seq-num exists and process the buffer
        if (strstr(read_buffer, "BAD") == NULL) {
            process_buffer(read_buffer);
        }
    }
}

void process_buffer(const char *read_buffer) {
    const char *line = read_buffer;
    char email_num[10];
    char subject[256];

    // Parse each email block in the buffer
    while ((line = strstr(line, "* "))) {
        // Find the email number
        sscanf(line, "* %s FETCH", email_num);

        // Find the subject line
        const char *subject_line = strstr(line, "Subject:");
        if (subject_line) {
            // Skip "Subject: "
            subject_line += 9;

            // Copy the subject into the subject buffer
            int i = 0;
            while (*subject_line != '\n' && *subject_line != '\r' && *subject_line != '\0') {
                subject[i++] = *subject_line++;
            }
            subject[i] = '\0';

            // Print the result in the desired format
            printf("%s: %s\n", email_num, subject);
        }

        // Move to the next block
        line = strstr(line, "FETCH") + 5;
    }
}