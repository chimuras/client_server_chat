#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

unsigned char* sending_types(unsigned char* command) {
    if (strcmp("/login", command) == 0) {
        return "LOGIN";
    } else if (strcmp("/createuser", command) == 0) {
        return "CREATEUSER";
    } else if (strcmp("/changepassword", command) == 0) {
        return "CHANGEPASSWORD";
    } else if (strcmp("/logout", command) == 0) {
        return "EXIT";
    } else if (strcmp("/joinsession", command) == 0) {
        return "JOIN";
    } else if (strcmp("/leavesession", command) == 0) {
        return "LEAVE_SESS";
    } else if (strcmp("/createsession", command) == 0) {
        return "NEW_SESS";
    } else if (strcmp("/invite", command) == 0) {
        return "INVITE";
    } else if (strcmp("/list", command) == 0) {
        return "QUERY";
    } else if (strcmp("/quit", command) == 0) {
        return "EXIT";
    } else if (strcmp("/texts", command) == 0) {
        return "MESSAGE";
    } else if (strcmp("/history", command) == 0) {
        return "HISTORY";

        //TEXTS:client_name:session_id:text messagesss
    } else {
        return "UNKNOWN";
    }
}

//unsigned char* received_types(unsigned char* packet) {
//    unsigned char* type;
//    int start = 0, next = 0;
//    findBounds_from_server(&start, &next, ':', packet);
//    type = get_string(start, next - 1, packet);
//    return type;
//
//}

void findBounds(int* start, int* end, char searchChar, unsigned char* info) {

    *start = *end;
    unsigned char * marker = strchr(info + *end, searchChar);
    if (marker == NULL) {
        //  printf("ntered Null \n");
        marker = strchr(info + *end, '\n');
    }
    *end = marker - info + 1;
}

void findBounds_from_server(int* start, int* end, char searchChar, unsigned char* info) {

    *start = *end;
    unsigned char * marker = strchr(info + *end, searchChar);
    if (marker == NULL) {
        //  printf("ntered Null \n");
        marker = strchr(info + *end, '\0');
    }
    *end = marker - info + 1;
}

unsigned char* get_string(int start, int end, unsigned char* string) {

    unsigned char* newString = malloc((sizeof (unsigned char))*(end - start + 1));
    for (int i = 0; i < end - start; i++) {
        newString[i] = string[start + i];
    }
    newString[end - start] = '\0';
    return newString;
}

unsigned char* combine_chars(unsigned char *s1, unsigned char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    unsigned char *result = malloc(len1 + len2); //+1 for the null-terminator
    //    unsigned char *result = malloc(len1 + len2); //+1 for the null-terminator
    //in real code you would check for errors in malloc here
    for (int a = 0; a < len1 + len2; a++) {

    }
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2); //+1 to copy the null-terminator
    return result;
}

unsigned char* create_packet(unsigned char* m1, unsigned char* m2, unsigned char* m3, unsigned char* m4) {
    unsigned char* m5, *m6, *m7, *m8, *m9, *m10;
    //    printf("wopfiej");
    if (m1 == NULL) {
        return NULL;
    }
    if (m2 == NULL) {
        return m1;
    } else {
        m5 = combine_chars(m1, ":");
        m6 = combine_chars(m5, m2);
    }
    if (m3 == NULL) {
        return m6;
    } else {
        m7 = combine_chars(m6, ":");
        m8 = combine_chars(m7, m3);
    }
    if (m4 == NULL) {
        //        puts(m8);
        return m8;
    } else {
        m9 = combine_chars(m8, ":");
        m10 = combine_chars(m9, m4);
        return m10;
    }

}