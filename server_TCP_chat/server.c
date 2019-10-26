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
#include "clientHandlers.h"
#include "session.h"
#include "fileHandlers.h"
#define MAXARGLEN 100
// chat-chatname.txt
// delete empty sessions
//gcc -Wall server.c clientHandlers.c  fileHandlers.c session.c -o server.out

//server.out server 5000

typedef struct messages {
    member* recievers;
    char* info;
} message;

client* people = NULL;
session* chats = NULL;

char* createPacket(char* type, int fragNo, char* info);
message handlePacket(char * packet);

#define BUFFER_LENGTH 2000

#define TRUE   1 
#define FALSE  0 
#define PORT 8888 
int tcp_socket;
struct sockaddr_in sender;
char* serverDirectory = "/nfs/ug/homes-2/c/chimuras/NetBeansProjects/server2/";

int main(int argc, char** argv) {
    processClientList(&people, "/nfs/ug/homes-2/c/chimuras/NetBeansProjects/server2/clientList.txt");
    struct sockaddr_in receiver;
    int sender_len = sizeof (sender), recv_len;
    unsigned char msg[BUFFER_LENGTH];

    //check for right amount of command
    //ignore argv[0]
    //argv[1] should be server
    //argv[2] should be UDP listen port number
    if (argc != 3) {
        printf("Amount of arguments wrong");
        return 1;
    }
    //check for right command
    if (strcmp(argv[1], "server")) {
        printf("Command not right, terminating program");
        return 1;
    }
    //create a socket
    if ((tcp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("Could not create a socket");
        return 1;
    }

    //clear 
    memset((char *) &receiver, 0, sizeof (receiver));
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(atoi(argv[2]));
    receiver.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if (bind(tcp_socket, (struct sockaddr*) &receiver, sizeof (receiver)) == -1) {
        printf("Failed to bind");
        return 1;
    }
    while (1) {
        if (recvfrom(tcp_socket, msg, BUFFER_LENGTH, 0, (struct sockaddr*) &sender, &sender_len) == -1) {
            printf("Failed to receive");
            //                return 1;
        }
        puts(msg);
        message returning_packet = handlePacket(msg);
        unsigned char *ack = returning_packet.info;
        member* listOfRecievers = returning_packet.recievers;
        puts(returning_packet.info);
        if (listOfRecievers == NULL) {
            printf("NULL FAM");
        }

        while (listOfRecievers != NULL) {

            int senderSockId = listOfRecievers->socketID;
            struct sockaddr_in senderSocket = listOfRecievers->socket;
            if (sendto(senderSockId, ack, strlen(ack), 0, (struct sockaddr*) & senderSocket, sizeof ( senderSocket)) == -1) {
                printf("Failed to send ack");
            }
            listOfRecievers = listOfRecievers->next;
        }
    }



    return (EXIT_SUCCESS);
}






message loginHandler(char* user, char* info);
message exitHandler(char* user, char* info);
message joinHandler(char* user, char* info);
message leaveSessHandler(char* user, char* info);
message queryHandler(char* user, char* info);
message newSessHandler(char* user, char* info);
message messageHandler(char* user, char* info);
message passHandler(char* user, char* info);
message addUserHandler(char* user, char* info);
message historyHandler(char* user, char* info);
message inviteHandler(char* user, char* info);
void recordMessage(char* chatName, char* user, char* message, char* mode);
void miniSendTo(char* msg, char* recieverName);
//char* createPacket(char* type,int fragNo, char* info);
char* getFilePath(char* sessionName);

message handlePacket(char * packet) {
    int start = 0;
    int end = 0;
    findBounds(&start, &end, ':', packet);
    char* command = getSubString(start, end - 1, packet);
    findBounds(&start, &end, ':', packet);
    char* user = getSubString(start, end - 1, packet);
    findBounds(&start, &end, '\0', packet);
    char* info = getSubString(start, end - 1, packet);

    if (strcmp(command, "LOGIN") == 0) {
        // return;
        return loginHandler(user, info);
    } else if (strcmp(command, "EXIT") == 0) {
        puts(packet);
        return exitHandler(user, info);
    }
    else if (strcmp(command, "CHANGEPASSWORD") == 0) {
        //  puts(packet);
        return passHandler(user, info);
    }
    else if (strcmp(command, "JOIN") == 0) {

        return joinHandler(user, info);
    }
    else if (strcmp(command, "LEAVE_SESS") == 0) {

        return leaveSessHandler(user, info);
    } else if (strcmp(command, "NEW_SESS") == 0) {

        return newSessHandler(user, info);
    }
    if (strcmp(command, "MESSAGE") == 0) {

        return messageHandler(user, info);
    }
    if (strcmp(command, "QUERY") == 0) {

        return queryHandler(user, info);
    }
    if (strcmp(command, "CREATEUSER") == 0) {

        return addUserHandler(user, info);
    }
    if (strcmp(command, "HISTORY") == 0) {

        return historyHandler(user, info);
    }
    if (strcmp(command, "INVITE") == 0) {

        return inviteHandler(user, info);
    }

}

message loginHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    client * a;
    a = search(people, user);

    joinMember(&(m.recievers), user, tcp_socket, sender);


    int start = 0, end = 0;
    findBounds(&start, &end, ':', info);
    char* password = getSubString(start, end - 1, info);

    int status = checkLogin(a, user, password);
    if (status == 1) {
        a->socketID = tcp_socket;
        (a->socket) = sender;
        findBounds(&start, &end, ':', info);
        a->ipAddr = getSubString(start, end - 1, info);
        findBounds(&start, &end, '\0', info);
        a->portAddress = getSubString(start, end - 1, info);
        m.info = "-LO_ACK";
    } else
        m.info = "-LO_NAK:SOME REASONS";
    // if status is not equal to 1 add reasons for fauilure
    return m;
}

message exitHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    puts(user);
    joinMember(&(m.recievers), user, tcp_socket, sender);
    if (logOut(people, user)) {
        removeUser(chats, user);
        m.info = "-LOGGED_OUT_ACK";
    } else
        m.info = "-LOGGED_OUT_NACK";

    return m;
}

message leaveSessHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    joinMember(&(m.recievers), user, tcp_socket, sender);
    int start = 0, end = 0;
    findBounds(&start, &end, ':', info);
    char* chatName = getSubString(start, end - 1, info);
    session* a = searchSession(chats, chatName);
    if (leaveSession(a, user)) {
        recordMessage(chatName, user, "LEFT SESSION", "ab");
        m.info = "-LV_ACK";

    } else
        m.info = "-LV_NACK";

    return m;
}

message joinHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    joinMember(&(m.recievers), user, tcp_socket, sender);

    if (search(people, user) != NULL) {
        int start = 0, end = 0;
        findBounds(&start, &end, ':', info);
        char* sessionName = getSubString(start, end - 1, info);
        session * b = searchSession(chats, sessionName);
        //printMemberList(&b->person);
        if (b != NULL) {
            if (joinMember(&b->person, user, tcp_socket, sender)) {
                recordMessage(sessionName, user, "JOINED SESSION", "ab");
                printMemberList(&b->person);
                m.info = "-JN_ACK";
            } else
                m.info = "-JN_NACK:Already Joined";
        } else {
            m.info = "-JN_NACK:Session doesn't exist";
        }
    } else
        m.info = "-JN_NACK:USER NOT LOGGED IN";
    return m;
}

message queryHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    joinMember(&(m.recievers), user, tcp_socket, sender);

    char* message = "-QU_ACK:Connected Users : ";
    message = combine_chars(message, getActiveClients(people));
    message = combine_chars(message, "\n");
    message = combine_chars(message, " Available Sessions : ");
    message = combine_chars(message, getActiveSessions(chats));
    message = combine_chars(message, "\n\n");
    m.info = message;
    return m;
}

message newSessHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    joinMember(&(m.recievers), user, tcp_socket, sender);
    int start = 0, end = 0;
    findBounds(&start, &end, ':', info);
    char* sessionName = getSubString(start, end - 1, info);
    if (insertSession(&chats, sessionName, user, tcp_socket, sender)) {
        recordMessage(sessionName, user, "CREATED THIS SESSION", "wb");
        m.info = "-NS_ACK";
    } else {
        m.info = "-NS_NACK:Already Created";
    }

    return m;
}

message messageHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    //puts(info);
    int start = 0, end = 0;
    findBounds(&start, &end, ':', info);
    char* sessionName = getSubString(start, end - 1, info);

    m.recievers = getMembers(chats, sessionName);
    if (inMemberList(&m.recievers, user)) {
        m.info = "-MESSAGED:";
        m.info = combine_chars(m.info, user);
        m.info = combine_chars(m.info, ":");
        m.info = combine_chars(m.info, sessionName);
        findBounds(&start, &end, '\n', info);
        char* message = getSubString(start, end - 1, info);
        m.info = combine_chars(m.info, ":");
        m.info = combine_chars(m.info, message);
        recordMessage(sessionName, user, message, "a");
    } else {
        m.info = "-MESSAGEFAIL:";
        m.recievers = NULL;
        joinMember(&(m.recievers), user, tcp_socket, sender);
    }
    return m;
}

message passHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    joinMember(&(m.recievers), user, tcp_socket, sender);
    client* a = search(people, user);
    if (a != NULL) {
        int start = 0, end = 0;
        findBounds(&start, &end, ':', info);
        char* currentPass = getSubString(start, end - 1, info);

        findBounds(&start, &end, '\0', info);
        char* newPass = getSubString(start, end - 1, info);

        if (strcmp(a->password, currentPass) == 0) {
            a->password = newPass;
            m.info = "-PASS_ACK:SUCESSFUL PASSWORD CHANGE";
        }
        else {
            m.info = "-PASS_NAK:ILLEGAL PASSWORD CHANGE";
            // illegal passsword change

        }

    }
    else {
        m.info = "-PASS_NAK:USER DOESNT EXIST";
    }

    return m;

}

message addUserHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    joinMember(&(m.recievers), user, tcp_socket, sender);
    client* a = search(people, user);
    if (a != NULL) {
        m.info = "-ADD_NAK:Username already used";
    }
    else {
        int start = 0, end = 0;
        findBounds(&start, &end, '\0', info);
        char* pass = getSubString(start, end - 1, info);
        insert(&people, user, pass);
        m = loginHandler(user, info);
        FILE *fileptr = fopen("/nfs/ug/homes-2/c/chimuras/NetBeansProjects/server2/clientList.txt", "w");
        changeClientList(&people, fileptr);
        fclose(fileptr);
    }

    return m;
}

void recordMessage(char* chatName, char* user, char* message, char* mode) {

    char* filePath = combine_chars(serverDirectory, "chat-");
    filePath = combine_chars(filePath, chatName);
    filePath = combine_chars(filePath, ".txt");
    FILE *fileptr;

    fileptr = fopen(filePath, mode);

    char* info = combine_chars(user, ":");
    info = combine_chars(info, message);
    info = combine_chars(info, "\n");
    fputs(info, fileptr);
    fclose(fileptr);
}

message historyHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    joinMember(&(m.recievers), user, tcp_socket, sender);
    int start = 0, end = 0;
    findBounds(&start, &end, '\0', info);
    char* sessionName = getSubString(start, end - 1, info);
    char* filePath = getFilePath(sessionName);
    
    FILE *fileptr;
    unsigned char *bb;
    unsigned long filelen;

    fileptr = fopen(filePath, "rb"); // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END); // Jump to the end of the file
    filelen = ftell(fileptr); // Get the current byte offset in the file
    fclose(fileptr);
    FILE* cc = fopen(filePath, "rb");
    bb = malloc((filelen + 1) * sizeof (unsigned char)); // Enough memory for file + \0

    if (fread(bb, sizeof (unsigned char), filelen / sizeof (unsigned char), cc) != filelen / sizeof (unsigned char)) {
    }
    fclose(cc); // Close the file
    FILE * i_file;
    i_file = fopen(filePath, "rb");
    unsigned long f_d = fileno(i_file);
    struct stat buffer;
    fstat(f_d, &buffer);
    unsigned long file_length = buffer.st_size;
    int total_fragments = (file_length / 1000) + 1;
    int last_packet_length = file_length % 1000;
    int sent = 0;
    unsigned char * message;
    unsigned char recvbuf[MAXARGLEN];
    //    printf("%d",total_fragments);
    for (int i = 1; i <= total_fragments; i++) {
        unsigned char* buf1;
        int sizeToRead = 1000;
        if (i == total_fragments) {
            sizeToRead = last_packet_length;
            buf1 = malloc((sizeToRead)*(sizeof (unsigned char)));
            for (int x = (i - 1)*1000; x < ((i - 1)*1000) + sizeToRead; x++) {
                buf1[x - ((i - 1)*1000)] = bb[x];
            }
        } else {
            buf1 = malloc((sizeToRead)*(sizeof (unsigned char)));
            for (int x = (i - 1)*1000; x < i * sizeToRead; x++) {
                buf1[x - ((i - 1)*1000)] = bb[x];
            }
        }
        //  -HISTORY_ACK: frago no:

        unsigned char size_of_buf_char[256];
        sprintf(size_of_buf_char, "%d", sizeToRead);
        message = createPacket("-HISTORY_ACK", i, buf1);
        sent = 0;
        miniSendTo(message, user);
    }

    m.info = "-HISTORY_ACK:0:Transfer done";
    return m;
}

message inviteHandler(char* user, char* info) {
    message m;
    m.recievers = NULL;
    m.info = NULL;
    int start = 0, end = 0;
    findBounds(&start, &end, ':', info);
    char* session = getSubString(start, end - 1, info);

    if (sessionCheck(chats, session, user)) {
        findBounds(&start, &end, ':', info);
        char* name = getSubString(start, end - 1, info);
        client* person = search(people, name);
        if (person != NULL && person->used) {
            miniSendTo("-INVITE_ACK", user);
            joinMember(&(m.recievers), person->username, person->socketID, person->socket);
            m.info = "-INVITED:";
            m.info = combine_chars(m.info, user);
            m.info = combine_chars(m.info, ":");
            m.info = combine_chars(m.info, session);
        } else {
            m.info = "-INVITE_NAK:";
            joinMember(&(m.recievers), user, tcp_socket, sender);
        }
    } else {
        joinMember(&(m.recievers), user, tcp_socket, sender);
        m.info = "-INVITE_NAK: Illegal Session";
    }

    return m;
}

void miniSendTo(char* msg, char* recieverName) {
    client* a = search(people, recieverName);
    if (sendto(a->socketID, msg, strlen(msg), 0, (struct sockaddr*) &a->socket, sizeof (a->socket)) == -1) {
        printf("Failed to send ack");
    }
}

char* createPacket(char* type, int fragNo, char* info) {
    unsigned char* m = combine_chars(type, ":");
    unsigned char msg[MAXARGLEN];
    sprintf(msg, "%d:", fragNo);
    unsigned char* m1 = combine_chars(m, msg);
    unsigned char* m2 = combine_chars(m1, info);

    return m2;
}

char* getFilePath(char* sessionName) {
    char* filePath = combine_chars(serverDirectory, "chat-");
    char* filePath1 = combine_chars(filePath, sessionName);
    char* filePath2 = combine_chars(filePath1, ".txt");
    if (!checkFileExist(filePath2)) {
        puts(filePath2);
            return NULL;
        }
    
    return filePath2;
}