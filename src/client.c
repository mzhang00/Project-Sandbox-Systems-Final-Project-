#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "proto.h"
#include "string.h"

volatile sig_atomic_t flag = 0;
int sockfd = 0;
char nickname[LENGTH_NAME];

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void recv_msg_handler() {
    char receiveMessage[LENGTH_SEND] = {};
    while (1) {
        int receive = recv(sockfd, receiveMessage, LENGTH_SEND, 0);
        if (receive > 0) {
            //printf("\r%s\n", receiveMessage);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        } else {
            // -1
        }
    }
}

void send_msg_handler() {
    char message[LENGTH_MSG] = {};
    while (1) {
        str_overwrite_stdout();
        while (fgets(message, LENGTH_MSG, stdin) != NULL) {
            str_trim_lf(message, LENGTH_MSG);
            if (strlen(message) == 0) {
                str_overwrite_stdout();
            } else {
                break;
            }
        }
        send(sockfd, message, LENGTH_MSG, 0);
        if (strcmp(message, "exit") == 0) {
            break;
        }
    }
    catch_ctrl_c_and_exit(2);
}

int main(int argc, char * argv[])
{
    signal(SIGINT, catch_ctrl_c_and_exit);

    // Naming
    printf("Please enter your name: ");
    fgets(nickname, LENGTH_NAME, stdin);

    // Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
        printf("Fail to create a socket.");
        exit(EXIT_FAILURE);
    }

    // Socket information
    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_info.sin_port = htons(8888);

    // Connect to Server
    int err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
    if (err == -1) {
        printf("Connection to Server error!\n");
        exit(EXIT_FAILURE);
    }

    // Names
    getsockname(sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
    getpeername(sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
    printf("Connect to Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    printf("You are: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

    send(sockfd, nickname, LENGTH_NAME, 0);

    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
        
        char name[500];
        if (argc > 1) {
            strcpy(name, argv[1]);
        }else{
            fgets(name, 500, stdin);
            int i = 0;
            while (name[i] != '\n') {
                i++;
        }
        name[i] = '\0';
        }

        if (strcmp(name, "check") == 0){

            FILE *f = fopen("log.txt", "r");
            char str[500];
            while (fgets(str, 500, f) != NULL){
                printf("%s\n", str);
            }
            fclose(f);

            }else if (strcmp(name, "clear") == 0){

            FILE *f = fopen("log.txt", "w");
            fclose(f);

            }else{
            FILE *f = fopen("log.txt", "a");

            fprintf(f, "%s\n", strcat(strcat(name, " "), nickname));

            fclose(f);
            
        }

    }
    

    close(sockfd);
    return 0;
}
