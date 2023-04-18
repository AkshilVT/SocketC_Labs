#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 5432
#define MAX_LINE 256

int main(int argc, char *argv[])
{
    FILE *fp;
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    char fileBuf[MAX_LINE];
    int s, l;
    int len;
    if (argc == 2)
    {
        host = argv[1];
    }
    else
    {
        fprintf(stderr, "usage: %s host\n", argv[0]);
        exit(1);
    }

    /* translate host name into peer's IP address */
    hp = gethostbyname(host);
    if (!hp)
    {
        fprintf(stderr, "%s: unknown host: %s\n", argv[0], host);
        exit(1);
    }
    else
        printf("Client's remote host: %s\n", argv[1]);

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT);

    /* active open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("simplex-talk: socket");
        exit(1);
    }
    else
        printf("Client created socket.\n");

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    }
    else
        printf("Client connected.\n");
    bzero(buf, sizeof(buf));
    recv(s, buf, sizeof(buf), 0);
    printf("\nClient received: %s \n", buf);
    printf("Type what you want to send to the server:");
    /* main loop: get and send lines of text */
    // while (fgets(buf, sizeof(buf), stdin))
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        buf[MAX_LINE - 1] = '\0';
        // Comparing if the user input is "get"
        if (strncmp(buf, "get", 3) == 0)
        {
            // Send the user input to the server
            send(s, buf, sizeof(buf), 0);
            bzero(buf, sizeof(buf));
            // Receive the message from the server
            recv(s, buf, sizeof(buf), 0);
            printf("\nClient received: %s \n", buf);
            bzero(buf, sizeof(buf));
            // Asking the user to input the filename
            printf("Type Filename the server:");
            fgets(buf, sizeof(buf), stdin);
            buf[MAX_LINE - 1] = '\0';
            strncpy(fileBuf, buf, sizeof(buf) - 1);
            printf("fileName: %s", fileBuf);
            send(s, buf, sizeof(buf), 0);
            // Receive the message from the server
            l = recv(s, buf, sizeof(buf), 0);
            printf("Received from server: %s\n", buf);
            // If the file is not found, the server will send "File not found" message
            if (strncmp("File not found", buf, sizeof("File not found")))
            {
                break;
            }
            // If the file is found, the server will send "File found" message
            else
            {
                // Open the file
                fp = fopen(fileBuf, "w");
                printf("Received: %s\n", buf);
                // Write the file
                fwrite(buf, sizeof(char), l, fp);
                // Receive the file from the server
                while (l = recv(s, buf, sizeof(buf), 0) > 0)
                {
                    printf("Received: %s\n", buf);
                    // Write the file
                    fwrite(buf, sizeof(char), l, fp);
                }
                fclose(fp);
            }
            printf("Transmitting file completed.\n");
        }
        // Comparing if the user input is "bye"
        if (strncmp(buf, "bye", 3) == 0)
        {
            printf("Client is exiting.\n");
            send(s, buf, sizeof(buf), 0);
            exit(0);
        }
        len = strlen(buf) + 1;
        send(s, buf, len, 0);
        printf("Type what you want to send to the server:");
    }
}
