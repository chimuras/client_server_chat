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
#include "functions.h"

#define BUFFER_LENGTH 2000

int main(int argc, char** argv) {
    struct sockaddr_in receiver;
    int client_socket, client_socket_len = sizeof (receiver);
    unsigned char buf[BUFFER_LENGTH];
    unsigned char *client_id, *password, *server_ip, *server_port;
    unsigned char *packet;
    unsigned char **sessions[BUFFER_LENGTH];
    fd_set read_fds;
    //    struct timeval timeout;
    if (argc != 2) {
        printf("Amount of arguments wrong");
        return 1;
    }

    if (strcmp(argv[1], "client")) {
        printf("Command not right, terminating program");
        return 1;
    }
    if ((client_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        return 1;
    }
    int terminate_program = 0;
    int logged_in = 0;
    int joined_session = 0;
    int current_session = 0;
    pid_t pid;

    while (!terminate_program) {
        char* commands;
        if (logged_in) {
            while (1) {
                //                FD_ZERO(&entered);
                FD_ZERO(&read_fds);
                FD_SET(STDIN_FILENO, &read_fds);
                FD_SET(client_socket, &read_fds);
                int max_size;
                if (STDIN_FILENO > client_socket) {
                    max_size = STDIN_FILENO + 1;
                } else {
                    max_size = client_socket + 1;
                }

                select(max_size, &read_fds, NULL, NULL, NULL);
                if (FD_ISSET(client_socket, &read_fds)) {
                    memset(buf, '\0', BUFFER_LENGTH);
                    if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                        printf("receiving from server FAILES\n");
                        return 1;
                    }
                    if (buf[1] == 'M') {
                        //                        puts(buf);
                        int start1 = 0, next1 = 0;
                        findBounds_from_server(&start1, &next1, ':', buf);
                        unsigned char* command = get_string(start1, next1 - 1, buf);
                        findBounds_from_server(&start1, &next1, ':', buf);
                        unsigned char* other_client = get_string(start1, next1 - 1, buf);
                        findBounds_from_server(&start1, &next1, ':', buf);
                        unsigned char* other_session = get_string(start1, next1 - 1, buf);
                        if (strcmp(sessions[current_session], other_session) != 0) {
                            printf("---------------%s---------------\n", other_session);
                            findBounds_from_server(&start1, &next1, '\0', buf);
                            unsigned char* texts = get_string(start1, next1 - 1, buf);
                            printf("                      [%s]\n", other_client);
                            printf("                        %s\n", texts);
                            printf("@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@@@@@@@@@@@\n", sessions[current_session]);
                        } else {
                            findBounds_from_server(&start1, &next1, '\0', buf);
                            unsigned char* texts = get_string(start1, next1 - 1, buf);
                            printf("                      [%s]\n", other_client);
                            printf("                        %s\n", texts);
                        }
                    }
                    if (buf[1] == 'I') {
                        int start1 = 0, next1 = 0;
                        findBounds_from_server(&start1, &next1, ':', buf);
                        unsigned char* command = get_string(start1, next1 - 1, buf);
                        findBounds_from_server(&start1, &next1, ':', buf);
                        unsigned char* other_client = get_string(start1, next1 - 1, buf);
                        findBounds_from_server(&start1, &next1, ':', buf);
                        unsigned char* other_session = get_string(start1, next1 - 1, buf);
                        printf("%s has invited you to join session <%s>\n", other_client, other_session);
                        

                    }
                }
                if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                    size_t nbytes = 1000;
                    commands = (char *) malloc(nbytes * sizeof (char));
                    size_t bytes_read = getline(&commands, &nbytes, stdin);
                    if (bytes_read == -1) {
                        puts("ERROR!");
                    } else {
                        break;
                    }
                }
            }
        } else {
            size_t nbytes = 1000;
            commands = (char *) malloc(nbytes * sizeof (char));
            size_t bytes_read = getline(&commands, &nbytes, stdin);
            if (bytes_read == -1) {
                puts("ERROR!");
            } else {
            }

        }
        int start = 0, next = 0;
        findBounds(&start, &next, ' ', commands);
        unsigned char* word = get_string(start, next - 1, commands);

        ///login rupan rupan 127.0.0.1 5000

        if (word[0] == '/') {//it is a command, and not a text message
            if (!logged_in) {
                if (strcmp("/quit", word) == 0) {
                    terminate_program = 1;
                    break;
                }
                if (strcmp("/login", word) == 0) {
                    findBounds(&start, &next, ' ', commands);
                    client_id = get_string(start, next - 1, commands);
                    findBounds(&start, &next, ' ', commands);
                    password = get_string(start, next - 1, commands);
                    findBounds(&start, &next, ' ', commands);
                    server_ip = get_string(start, next - 1, commands);
                    findBounds(&start, &next, ' ', commands);
                    server_port = get_string(start, next - 1, commands);
                    memset((char *) &receiver, 0, sizeof (receiver));
                    receiver.sin_family = AF_INET;
                    receiver.sin_port = htons(atoi(server_port));
                    if (inet_aton(server_ip, &receiver.sin_addr) == 0) {
                        fprintf(stderr, "inet_aton() failed FOR IP\n");
                        return 1;
                    }
                    packet = create_packet(sending_types(word), client_id, password, NULL);
                    if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, sizeof (receiver)) == -1) {
                        printf("sending to server FAILES\n");
                    } else {
                    }
                    memset(buf, '\0', BUFFER_LENGTH);
                    if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                        printf("receiving from server FAILES\n");
                        return 1;
                    }
                    int start_ack = 0, next_ack = 0;
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* received_type = get_string(start_ack, next_ack - 1, buf);
                    if (strcmp("-LO_ACK", received_type) == 0) {
                        printf("Logged IN\n");
                        logged_in = 1;
                        continue;
                    } else if (strcmp("-LO_NAK", received_type) == 0) {
                        //                        puts(received_type);
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        unsigned char* reason
                                = get_string(start_ack, next_ack - 1, buf);
                        printf("%s\n", reason);
                        printf("Either your entered client ID or password is not registered in the client list\n");
                        //                        scanf("%s %s", client_id, password);
                    } else {
                        printf("Unknown ACK received from server\n");
                    }
                } else if (strcmp("/createuser", word) == 0) {
                    findBounds(&start, &next, ' ', commands);
                    client_id = get_string(start, next - 1, commands);
                    findBounds(&start, &next, ' ', commands);
                    password = get_string(start, next - 1, commands);
                    findBounds(&start, &next, ' ', commands);
                    server_ip = get_string(start, next - 1, commands);
                    findBounds(&start, &next, ' ', commands);
                    server_port = get_string(start, next - 1, commands);
                    memset((char *) &receiver, 0, sizeof (receiver));
                    receiver.sin_family = AF_INET;
                    receiver.sin_port = htons(atoi(server_port));
                    if (inet_aton(server_ip, &receiver.sin_addr) == 0) {
                        fprintf(stderr, "inet_aton() failed FOR IP\n");
                        return 1;
                    }
                    packet = create_packet(sending_types(word), client_id, password, NULL);
                    //                    printf("<%s><%s>\n",client_id,password);
                    if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, sizeof (receiver)) == -1) {
                        printf("sending to server FAILES\n");
                        //                        return 1;
                    } else {
                    }
                    //                    puts(packet);
                    memset(buf, '\0', BUFFER_LENGTH);
                    if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                        printf("receiving from server FAILES\n");
                        return 1;
                    }

                    int start_ack = 0, next_ack = 0;
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* received_type = get_string(start_ack, next_ack - 1, buf);
                    if (strcmp("-LO_ACK", received_type) == 0) {
                        printf("Logged IN\n");
                        logged_in = 1;
                        continue;
                    } else if (strcmp("-ADD_NAK", received_type) == 0) {
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        unsigned char* reason
                                = get_string(start_ack, next_ack - 1, buf);
                        printf("%s\n", reason);
                        printf("Couldn't create a new account\n");
                    } else {
                        printf("Unknown ACK received from server\n");
                    }
                } else {
                    printf("Please LOG IN first. \n");
                }
            } else {
                if ((strcmp("/createsession", word) == 0) || (strcmp("/joinsession", word) == 0) || (strcmp("/leavesession", word) == 0)) {
                    findBounds(&start, &next, ' ', commands);
                    unsigned char* session_id;
                    session_id = get_string(start, next - 1, commands);
                    if (strcmp("/createsession", word) == 0) {
                        packet = create_packet(sending_types(word), client_id, session_id, NULL);
                    } else if (strcmp("/joinsession", word) == 0) {
                        packet = create_packet(sending_types(word), client_id, session_id, NULL);
                    } else if (strcmp("/leavesession", word) == 0) {
                        if (joined_session <= 0) {
                            puts("You are not in any sessions.\n");
                            printf("Please JOIN SESSION first. \n");
                            continue;
                        }
                        packet = create_packet(sending_types(word), client_id, session_id, NULL);
                    }
                    if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, client_socket_len) == -1) {
                        printf("sending to server FAILES\n");
                        return 1;
                    }
                    memset(buf, '\0', BUFFER_LENGTH);
                    if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                        printf("receiving from server FAILES\n");
                        return 1;
                    }
                    int start_ack = 0, next_ack = 0;
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* received_type
                            = get_string(start_ack, next_ack - 1, buf);
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* s_id
                            = get_string(start_ack, next_ack - 1, buf);
                    if (strcmp("-NS_ACK", received_type) == 0) {
                        printf("Successfully CREATED session <%s>\n", session_id);
                        sessions[joined_session] = session_id;
                        current_session = joined_session;
                        printf("@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@@@@@@@@@@@\n", sessions[current_session]);
                        joined_session++;
                        continue;
                    } else if (strcmp("-JN_ACK", received_type) == 0) {
                        printf("Successfully JOINED session <%s>\n", session_id);
                        sessions[joined_session] = session_id;
                        current_session = joined_session;
                        printf("@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@@@@@@@@@@@\n", sessions[current_session]);
                        joined_session++;
                        continue;
                    } else if (strcmp("-LV_ACK", received_type) == 0) {
                        printf("Successfully LEAVED session <%s>\n", session_id);
                        int change = 0;
                        for (unsigned int x = 0; x < joined_session; x++) {
                            if (strcmp(sessions[x], session_id) == 0) { 
                                change=1;
                            }
                            if(change){
                                if((x+1)>=joined_session){
                                    current_session=x-1;
                                    sessions[x]="\0";
                                    break;
                                }
                                unsigned char* n;
                                n=sessions[x+1];
                                sessions[x]=n;
                            }
                        }
                        if (change) {
                            joined_session--;
                            if (joined_session == 0) {
                                puts("You are not in any sessions right now.\n");
                            } else {
                                printf("You are now in session <%s>\n", sessions[current_session]);
                                printf("@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@@@@@@@@@@@\n", sessions[current_session]);
                            }
                        }
                        continue;

                    } else if (strcmp("-JN_NACK", received_type) == 0) {
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        unsigned char* reason
                                = get_string(start_ack, next_ack - 1, buf);
                        printf("FAILED to join session <%s>:%s\n", session_id, reason);
                    } else if (strcmp("-LV_NACK", received_type) == 0) {
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        printf("Could not leave session <%s>", session_id);
                    } else if (strcmp("-NS_NACK", received_type) == 0) {
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        unsigned char* reason
                                = get_string(start_ack, next_ack - 1, buf);
                        printf("Could NOT create session <%s>\n", session_id);
                        //                        printf("%s\n", reason);
                    } else {
                        printf("Unknown ACK received from server\n");
                    }

                } else if ((strcmp("/logout", word) == 0) || (strcmp("/list", word) == 0) || (strcmp("/quit", word) == 0)) {
                    packet = create_packet(sending_types(word), client_id, NULL, NULL);
                    if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, client_socket_len) == -1) {
                        printf("sending to server FAILES\n");
                        return 1;
                    }
                    memset(buf, '\0', BUFFER_LENGTH);
                    if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                        printf("receiving from server FAILES\n");
                        return 1;
                    }
                    int start_ack = 0, next_ack = 0;
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* received_type
                            = get_string(start_ack, next_ack - 1, buf);
                    if (strcmp("-LOGGED_OUT_ACK", received_type) == 0) {
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        unsigned char* c_id
                                = get_string(start_ack, next_ack - 1, buf);
                        printf("Successfully LOGGED_OUT \n");
                        logged_in = 0;
                        for (unsigned int x = 0; x < joined_session; x++) {
                            memset(sessions[x], '\0', BUFFER_LENGTH);
                        }
                        joined_session = 0;
                        if (strcmp("/quit", word) == 0) {
                            puts("Program Terminated\nGOOD BYE and HAVE a MAGNIFICENT DAYYY SWAAAAAGGG");
                            terminate_program == 1;
                            break;

                        }
                    } else if (strcmp("-QU_ACK", received_type) == 0) {
                        start_ack = next_ack;
                        next_ack = sizeof (buf);
                        unsigned char* lists
                                = get_string(start_ack, next_ack - 1, buf);
                        printf("%s", lists);
                    } else {

                    }
                } else if (strcmp("/changesession", word) == 0) {
                    findBounds(&start, &next, ' ', commands);
                    unsigned char* session_id;
                    session_id = get_string(start, next - 1, commands);
                    if (joined_session > 0) {
                        for (unsigned int x = 0; x < joined_session; x++) {
                            if (strcmp(sessions[x], session_id) == 0) {
                                current_session = x;
                                printf("You are now in session <%s>\n", session_id);
                                printf("@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@@@@@@@@@@@\n", sessions[current_session]);
                                break;
                            }
                            if (x == joined_session - 1) {
                                puts("You are either not in this session, or this session doesn't exist.\n");
                            }
                        }
                    }
                } else if (strcmp("/invite", word) == 0) {
                    if (joined_session > 0) {
                        findBounds(&start, &next, ' ', commands);
                        unsigned char* inviting_id;
                        inviting_id = get_string(start, next - 1, commands);
                        findBounds(&start, &next, ' ', commands);
                        unsigned char* inviting_session_id;
                        inviting_session_id = get_string(start, next - 1, commands);
                        int found = 0;
                        for (unsigned int x = 0; x < joined_session; x++) {
                            if (strcmp(sessions[x], inviting_session_id) == 0) {
                                found = 1;
                                break;
                            }
                        }
                        if (found == 0) {
                            printf("You are not in the session <%s>, you cannot invite user.\n", inviting_session_id);
                            continue;
                        }
                        packet = create_packet(sending_types(word), client_id, inviting_session_id, inviting_id);
                        if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, client_socket_len) == -1) {
                            printf("sending to server FAILES\n");
                            return 1;
                        }
                        memset(buf, '\0', BUFFER_LENGTH);
                        if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                            printf("receiving from server FAILES\n");
                            return 1;
                        }
                        int start_ack = 0, next_ack = 0;
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        unsigned char* received_type
                                = get_string(start_ack, next_ack - 1, buf);
                        if (strcmp("-INVITE_ACK", received_type) == 0) {
                            findBounds_from_server(&start_ack, &next_ack, ':', buf);
                            unsigned char* c_id
                                    = get_string(start_ack, next_ack - 1, buf);
                            printf("Successfully INVITED \n");
                        } else if (strcmp("-INVITE_NAK", received_type) == 0) {
                            unsigned char* lists
                                    = get_string(start_ack, next_ack - 1, buf);
                            printf("Couldn't invite. %s\n", lists);
                        } else {

                        }
                    } else {
                        puts("Please join session first");
                    }
                } else if (strcmp("/history", word) == 0) {
                    findBounds(&start, &next, ' ', commands);
                    unsigned char* session_id;
                    session_id = get_string(start, next - 1, commands);
                    if (joined_session > 0) {
                        for (unsigned int x = 0; x < joined_session; x++) {
                            if (strcmp(sessions[x], session_id) == 0) {
                                int done_transfer = 0;
                                printf("History for session <%s>\n\n", sessions[x]);
                                packet = create_packet(sending_types(word), client_id, sessions[x], NULL);
                                if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, client_socket_len) == -1) {
                                    printf("sending to server FAILES\n");
                                    return 1;
                                }
                                while (!done_transfer) {
                                    memset(buf, '\0', BUFFER_LENGTH);
                                    if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                                        printf("receiving from server FAILES\n");
                                        return 1;
                                    }
                                    int start_ack = 0, next_ack = 0;
                                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                                    unsigned char* received_type
                                            = get_string(start_ack, next_ack - 1, buf);
                                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                                    unsigned char* frag_number
                                            = get_string(start_ack, next_ack - 1, buf);
                                    findBounds_from_server(&start_ack, &next_ack, '\0', buf);
                                    unsigned char* recieved_history
                                            = get_string(start_ack, next_ack - 1, buf);
                                    if (strcmp("-HISTORY_ACK", received_type) == 0) {
                                        if (strcmp(frag_number,"0")==0) {
                                            printf("\nEnd of history for session <%s>\n", sessions[x]);
                                            printf("@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@@@@@@@@@@@\n", sessions[current_session]);
                                            done_transfer=1;
                                            break;
                                        }
                                        printf("%s", recieved_history);
                                    }
                                }
                                continue;
                            }
                            if(x==joined_session-1){
                              puts("You are either not in this session, or this session doesn't exist.\n");  
                            }
                        }
                        
                    } else {
                        puts("You are not in any sessions.\n");
                        printf("Please JOIN SESSION first. \n");
                        continue;
                    }
                } else if (strcmp("/changepassword", word) == 0) {
                    printf("Enter the current password to verify: ");
                    unsigned char* current_pass;
                    scanf("%s", current_pass);
                    if (strcmp(current_pass, password) != 0) {
                        puts("The password you entered did not match the current password.");
                        continue;
                    }
                    printf("Verified. Enter the new password: ");
                    unsigned char* new_pass;
                    scanf("%s", new_pass);
                    packet = create_packet(sending_types(word), client_id, current_pass, new_pass);
                    if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, client_socket_len) == -1) {
                        printf("sending to server FAILES\n");
                        return 1;
                    }
                    memset(buf, '\0', BUFFER_LENGTH);
                    if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                        printf("receiving from server FAILES\n");
                        return 1;
                    }
                    int start_ack = 0, next_ack = 0;
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* received_type = get_string(start_ack, next_ack - 1, buf);
                    if (strcmp("-PASS_ACK", received_type) == 0) {
                        printf("Password SUCCESSFULLY CHANGED\n");
//                        password = (char*) realloc(password, strlen(new_pass));
                        password=new_pass;
//                        strcpy(password, new_pass);
                        continue;
                    } else if (strcmp("-PASS_NACK", received_type) == 0) {
                        findBounds_from_server(&start_ack, &next_ack, ':', buf);
                        unsigned char* c_id = get_string(start_ack, next_ack - 1, buf);
                        printf("%s", c_id);
                        continue;
                    }
                } else {
                    printf("The command <%s> is unknown\n ", word);
                }
            }
        } else {
            if (!logged_in) {
                printf("Please LOG IN first. \n");
                continue;
            } else {
                if (joined_session == 0) {
                    printf("Please JOIN SESSION first. \n");
                    continue;
                }
                packet = create_packet(sending_types("/texts"), client_id, sessions[current_session], commands);
                //                puts(packet);
                if (sendto(client_socket, packet, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, client_socket_len) == -1) {
                    printf("sending to server FAILES\n");
                    return 1;
                }
                memset(buf, '\0', BUFFER_LENGTH);
                if (recvfrom(client_socket, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &receiver, &client_socket_len) == -1) {
                    printf("receiving from server FAILES\n");
                    return 1;
                }
                int start_ack = 0, next_ack = 0;
                findBounds_from_server(&start_ack, &next_ack, ':', buf);
                unsigned char* received_type
                        = get_string(start_ack, next_ack - 1, buf);
                if (strcmp("-MESSAGED", received_type) == 0) {
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* c_id = get_string(start_ack, next_ack - 1, buf);
                } else if (strcmp("-MESSAGEFAIL", received_type) == 0) {
                    findBounds_from_server(&start_ack, &next_ack, ':', buf);
                    unsigned char* c_id = get_string(start_ack, next_ack - 1, buf);
                    puts("Could not send message\n");
                } else {
                    printf("Problem with Message?\n");
                }

            }
        }
    }
    return (EXIT_SUCCESS);
}