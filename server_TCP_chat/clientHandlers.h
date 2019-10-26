/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef CLIENTHANDLERS_H
#define CLIENTHANDLERS_H

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

#define BADPASS 4
#define NOUSER 3
#define LOGINALREADY 2
#define LOGINSUCCESS 1
    

typedef struct clientEntry{
    char* username;
    char* password;
    char* lastSession;
    int used; // run ttime
    int socketID;
    struct sockaddr_in socket; // after successfull login
    char* ipAddr; // after successfull login
   char* portAddress; // after successfull login
    struct clientEntry * left; //null
    struct clientEntry * right; //null
} client ;
    
void insert(client**  tree, char* username, char* password);
void printAllClients(client* tree);

void printActiveClients(client* tree);
char*  getActiveClients(client* tree);
int logOut(client* tree,char * name);

int checkLogin(client* tree,char * name,char * pass);
client* search(client* tree,char * name);
client*  createClientEntry( char* username, char* password);
void processClientList(client**  a,char* filePath);
void changeClientList(client**  a, FILE *fileptr);


#endif /* CLIENTHANDLERS_H */

