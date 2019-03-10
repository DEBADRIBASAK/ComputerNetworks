#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    //Stream sockets and rcv()

    struct addrinfo hints, *res;
    int sockfd;

    char buf[2056];
    int byte_count;

    //get host info, make socket and connect it
    memset(&hints, 0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo("www.google.com","80", &hints, &res);
    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    printf("Connecting...\n");
    connect(sockfd,res->ai_addr,res->ai_addrlen);
    printf("Connected!\n");
    char *header = "GET /search?source=hp&ei=w-qDXPbdDtm_rQGmgqpw&q=send+http+request+to+google+using+c+sockets&btnK=Google+Search&oq=send+http+request+to+google+using+c+sockets&gs_l=psy-ab.3...244.6856..7427...0.0..0.108.1313.12j2......0....1..gws-wiz.......35i39j0i67j0i131j0j0i20i263j35i304i39.J2L4fg6M9MQ HTTP/1.1\nHost: www.google.com\r\n\r\n";
    send(sockfd,header,sizeof header,0);
    printf("GET Sent...\n");
    //all right ! now that we're connected, we can receive some data!
    byte_count = recv(sockfd,buf,sizeof(buf)-1,0); // <-- -1 to leave room for a null terminator
    buf[byte_count] = '\0'; // <-- add the null terminator
    printf("recv()'d %d bytes of data in buf\n",byte_count);
    printf("%s",buf);
    return 0;
}