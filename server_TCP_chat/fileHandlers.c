/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
# include "fileHandlers.h"

int checkFileExist(unsigned char* filePath) {

    if (access(filePath, F_OK) == -1) {
        return 0;
    }
    return 1;
}


void findBounds(int* start, int* end, char searchChar, unsigned char* info) {

    *start = *end;
    unsigned char * marker = strchr(info + *end, searchChar);
    if (marker == NULL) {
        //  printf("ntered Null \n");
        marker = strchr(info + *end, '\0');
    }
    *end = marker - info + 1;
}

unsigned char* getSubString(int start, int end, unsigned char* string) {

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
    unsigned char *result = malloc(len1 + len2 + 1); //+1 for the null-terminator
    //    unsigned char *result = malloc(len1 + len2); //+1 for the null-terminator
    //in real code you would check for errors in malloc here
    for (int a = 0; a < len1 + len2; a++) {

    }
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); //+1 to copy the null-terminator
    return result;
}