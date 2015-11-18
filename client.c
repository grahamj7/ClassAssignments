/* CMPT 332 - Fall 2015
 * Assignment 4, Question 1
 *
 * Jordaen Graham - jhg257
 * Jennifer Rospad - jlr247
 *
 * File: client.c 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "server.h"
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    char *host;
    
    /* todo */
    if (argc != 2) {
        host = "localhost";
    } else {
        host = argv[1];
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure


    printf("Here\n");

    ssize_t size = 0, *n;
    int a;
    char tempstr[MAXDATASIZE];
    socklen_t sin_size;
    
//    int new_fd = accept(sockfd, (struct sockaddr *)p->ai_addr, p->ai_addrlen);
    
    while(1){
        printf("while\n");
//        scanf("%d", &a);
//        printf("Retrieved %d\n", a);
        size = scanf("%s", tempstr); // store a string
        printf("got input: %s, size: %d\n", tempstr, size);
        if (0 < size) {
            if(strcmp(tempstr, "quit") == 0)
                break;
            if(send(sockfd, tempstr, strlen(tempstr), 0) == -1) {
                fprintf(stderr, "error sending the string: %s\n", tempstr);
            }else{
                printf("Message sent\n");
            }
        }
    }
    
    printf("Broke out of while loop;\nWaiting to receive data\n\n");
    
    
    
    
    
    
    
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(sockfd);

    return 0;
}
