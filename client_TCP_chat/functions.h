#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

unsigned char* sending_types(unsigned char* command);
//unsigned char* received_types(unsigned char* packet);
void findBounds(int* start, int* next, char searchChar, unsigned char* info);
void findBounds_from_server(int* start, int* end, char searchChar, unsigned char* info);
unsigned char* get_string(int start, int end, unsigned char* string);
unsigned char* combine_chars(unsigned char *s1, unsigned char *s2);
unsigned char* create_packet(unsigned char* m1, unsigned char* m2, unsigned char* m3, unsigned char* m4);

#endif /* FUNCTIONS_H */

