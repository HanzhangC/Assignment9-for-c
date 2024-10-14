#include "functions.h"

void handle_retrieve(int sockfd, char* msgNum) {
    char write_buffer[BUFFER_SIZE];
    snprintf(write_buffer, BUFFER_SIZE, "H003 FETCH %s BODY.PEEK[]\r\n", msgNum);
    send_msg(sockfd, write_buffer);
    
    char one_line[BUFFER_SIZE]; 
    char c;
    int num_char = 0;

    // read the fetch line, get the string and put it into one_line
    while(1) {
        read(sockfd, &c, sizeof(char));
        one_line[num_char] = c;
        num_char ++;
        if(c == '\n') {
            one_line[num_char] = '\0';
            break;
        }
    }

    // Check if seq-num exists
    if (strstr(one_line, "BAD") != NULL) {
        // Handle the case where seq-num does not exist
        printf("Message not found\n");
        exit(3); // Or perform other actions as needed
    }

    // get the number
    size_t i;
    int start = 0, end = 0;
    for(i = 0; i < strlen(one_line); i++) {
        if(one_line[i] == '{') {
            start = i + 1;
        }
        if(one_line[i] == '}') {
            end = i;
            break;
        }
    }
    one_line[end] = '\0';

    int total = atoi(one_line + start);
    while(total > 0) {
        total -= 1;
        read(sockfd, &c, sizeof(char));
        printf("%c", c);
    }
}