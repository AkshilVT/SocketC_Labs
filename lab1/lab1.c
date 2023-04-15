/*------------------------------------
    * lab1.c
    *------------------------------------
    AUTHOR:  Akshil Thumar
    SEMESTER:  Fall 2023
------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Function to print usage
void usage(void);

int main(int argc, char **argv)
{

    FILE *src;
    FILE *dest;
    char buf[100];
    char destbuf[100];
    char c;
    char readFIle;
    pid_t pid;
    // Function to execute a command (terminal command)
    int system(const char *command);

    // Default file names
    strcpy(buf, "sample.txt");
    strcpy(destbuf, "sample_out.txt");

    // Check if user provided the correct arguments
    while ((c = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (c)
        {
        case 'i':
            strcpy(buf, optarg);
            printf("Input file: %s\n", buf);
            break;
        case 'o':
            strcpy(destbuf, optarg);
            printf("Output file: %s\n", destbuf);
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
        }
    }

    // Open the files
    src = fopen(buf, "r");
    dest = fopen(destbuf, "w");

    // Creating a child process
    if (fork() != 0)
    {
        // Parent process waits for child to finish
        wait(NULL);
        // Get the process ID
        pid = getpid();
        printf("Parent PID: %d\n", pid);
        // Check if the file exists
        if (src == NULL)
        {
            printf("File not found. Exiting.\n");
            exit(EXIT_FAILURE);
        }
        printf("File found. Copying...\n");
        // Copy the file
        while ((readFIle = fgetc(src)) != EOF)
        {
            fputc(readFIle, dest);
        }
    }
    // Child processes
    else
    {
        // Child 1
        if (fork() == 0)
        {
            wait(NULL);
            pid = getpid();
            printf("Child 2 PID: %d\n", pid);
            system("ls -la");
        }
        // Child 2
        else
        {
            pid = getpid();
            printf("Child 1 PID: %d\n", pid);
            system("date");
            wait(NULL);
        }
    }
    // Close the files
    fclose(src);
    fclose(dest);
    return 0;
}

// Incase user does not provide the correct arguments
void usage(void)
{

    printf("Usage:\n");
    printf(" -i <input file>\n");
    printf(" -o <output file>\n");
    exit(8);
}