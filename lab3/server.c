#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

int main(int argc, char *argv[])
{
    FILE *fp;
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    char fileName[MAX_LINE];
    socklen_t len;
    int s, new_s;
    char str[INET_ADDRSTRLEN];
    char *host;
    struct hostent *hp;

    /* If socket IP address specified, bind to it. */
    host = "0.0.0.0";
    if (argc == 2)
    {
        host = argv[1];
    }
    /* Else bind to 0.0.0.0 */
    else
        sin.sin_addr.s_addr = INADDR_ANY;

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    // convert IPv4 and IPv6 addresses from text to binary form
    sin.sin_addr.s_addr = inet_addr(host);
    // Endian format converting to network byte order
    sin.sin_port = htons(SERVER_PORT);

    /* setup passive open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("simplex-talk: socket");
        exit(1);
    }
    // convert IPv4 and IPv6 addresses from text to binary form storing it in str
    inet_ntop(AF_INET, &(sin.sin_addr), str, INET_ADDRSTRLEN);
    printf("Server is using address %s and port %d.\n", str, SERVER_PORT);

    // bind the socket to an address
    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
        perror("simplex-talk: bind");
        exit(1);
    }
    else
        printf("Server bind done.\n");

    // listen for connections on a socket
    listen(s, MAX_PENDING);

    /* wait for connection, then receive and print text */
    while (1)
    {
        // accept a connection on a socket
        if ((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0)
        {
            perror("simplex-talk: accept");
            exit(1);
        }
        printf("Server Listening.\n");
        // Sending Hello to the client!
        send(new_s, "Hello", 5, 0);
        while (len = recv(new_s, buf, sizeof(buf), 0))
        {
            printf("Client said: %s ", buf);
            // Comparing if client sends GET send OK!
            if (strncmp(buf, "get", 3) == 0)
            {

                send(new_s, "OK", 2, 0);
                bzero(buf, sizeof(buf));

                // receive file name from client
                recv(new_s, buf, sizeof(buf), 0);
                strncpy(fileName, buf, strlen(buf) - 1);
                printf("\nfile requested: %s\n", fileName);
                fp = fopen(fileName, "r");
                // If file not found send error message!
                if (fp == NULL)
                {
                    printf("File open error\n");
                    send(new_s, "File not found", sizeof("File not found"), 0);
                    break;
                }
                else
                {
                    // Sending file to client!
                    printf("Client requested file: %s", buf);
                    while (!feof(fp))
                    {
                        bzero(buf, sizeof(buf));
                        fread(buf, 1, MAX_LINE, fp);
                        printf("Sending file to client: %s\n", buf);
                        send(new_s, buf, sizeof(buf), 0);
                    }
                    fclose(fp);
                    bzero(fileName, sizeof(fileName));
                    printf("File sent to client.\n");
                    break;
                }
            }
            // Comparing if client sends BYE disconnect!
            if (strncmp(buf, "bye", 3) == 0)
            {
                printf("Client Disconnected.\n");
                break;
            }
            bzero(buf, sizeof(buf));
        }
        close(new_s);
    }
}
