/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef SESSION_H
#define SESSION_H
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
   typedef  struct members{
       char* client;
       int socketID;
        struct sockaddr_in  socket;
       struct members* next;
   
   } member;
    
    
typedef struct sessions{
    char* sessionName;
    member* person;
    struct session * left; //null
    struct session * right; //null

} session ;
    
int insertSession(session** a,  char* sessionName, char* username, int socketID,  struct sockaddr_in socket);
void printSessions(session* tree);
session* searchSession(session* tree,char * name);
char*  getActiveSessions(session* tree);
int joinMember(member** a,char* username, int socketID,  struct sockaddr_in  socket);
void printMemberList(member** a);
int sessionCheck(session*  tree,char* sessionName, char* userName);
void deleteSession(session**  tree, char* sessionName, char* username);
int leaveSession(session* b, char* username);
void removeUser(session*  tree, char* username);
member* getMembers(session*  tree, char* sessionName);
int userLeave(member** group ,char* username);
int inMemberList(member** a,char* userName);


#endif /* SESSION_H */

