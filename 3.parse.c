#include "functions.h"

void handle_parse(int sockfd, char* msgNum) {
    char write_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];

    // Send commands to retrieve different parts of the email header information
    snprintf(write_buffer, BUFFER_SIZE, "H004 FETCH %s BODY.PEEK[HEADER.FIELDS (FROM)]\r\n", msgNum);
    send_msg(sockfd, write_buffer);
    receive_msg(sockfd, read_buffer, BUFFER_SIZE);
    
    // Find and output the contents of the "From" header field
    char *from_start = strcasestr_custom(read_buffer, "From:");
    if (from_start != NULL) {
        char *from_end = strstr(from_start, "\r\n");
        if (from_end != NULL) {
            *from_end = '\0'; // Change '\n' to '\0'
            printf("From: %s\n", from_start + 6);
        }
    }

    snprintf(write_buffer, BUFFER_SIZE, "H004 FETCH %s BODY.PEEK[HEADER.FIELDS (TO)]\r\n", msgNum);
    send_msg(sockfd, write_buffer);
    receive_msg(sockfd, read_buffer, BUFFER_SIZE);
    
    // Find and output the contents of the "To" header field
    char *to_start = strcasestr_custom(read_buffer, "To:");
    if (to_start != NULL) {
        char *to_end = strstr(to_start, "\r\n");
        if (to_end != NULL) {
            *to_end = '\0'; // Change '\n' to '\0'
            printf("To: %s\n", to_start + 4);
        }
    } else {
        printf("To:\n");
    }

    snprintf(write_buffer, BUFFER_SIZE, "H004 FETCH %s BODY.PEEK[HEADER.FIELDS (DATE)]\r\n", msgNum);
    send_msg(sockfd, write_buffer);
    receive_msg(sockfd, read_buffer, BUFFER_SIZE);
    
    // Find and output the contents of the "Date" header field
    char *date_start = strcasestr_custom(read_buffer, "Date:");
    if (date_start != NULL) {
        char *date_end = strstr(date_start, "\r\n");
        if (date_end != NULL) {
            *date_end = '\0'; // Change '\n' to '\0'
            printf("Date: %s\n", date_start + 6);
        }
    }

    snprintf(write_buffer, BUFFER_SIZE, "H004 FETCH %s BODY.PEEK[HEADER.FIELDS (SUBJECT)]\r\n", msgNum);
    send_msg(sockfd, write_buffer);
    receive_msg(sockfd, read_buffer, BUFFER_SIZE);
    
    // Find and output the contents of the "Subject" header field
    char *subject_start = strcasestr_custom(read_buffer, "Subject:");
    if (subject_start != NULL) {
        char *subject_mid = strchr(subject_start, '\r'); // Find the first '\r'
        if (subject_mid != NULL) {
            *subject_mid = '\0'; // Change first '\r' to '\0'
            char *subject_second_line_start = strchr(subject_mid + 1, '\n'); // Find the first '\n'
            if (subject_second_line_start != NULL) {
            char *subject_second_line_end = strchr(subject_second_line_start + 1, '\r'); // Find the second '\r'
                if (subject_second_line_end != NULL) {
                    *subject_second_line_end = '\0'; // Change second '\r' to '\0'
                    printf("Subject: %s%s\n", subject_start + 9, subject_second_line_start + 1);
                }
            }
        }
    } else {
        printf("Subject: <No subject>\n");
    }
}

// An ignore-capitals function
char *strcasestr_custom(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    for (const char *p = haystack; *p; p++) {
        if (tolower(*p) == tolower(*needle)) {
            for (size_t i = 1;; i++) {
                if (!needle[i]) return (char *)p;
                if (tolower(p[i]) != tolower(needle[i])) break;
            }
        }
    }
    return NULL;
}