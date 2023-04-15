/*
   1. Understand the following code.
   2. Print a character n times where the character and n are
      passed as command-line arguments.
*/

/*------------------------------------
    * lab1.c
    *------------------------------------
    AUTHOR:  Akshil Thumar
    SEMESTER:  Fall 2023
------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Incase user does not provide the correct arguments
void usage(void);

int main(int argc, char **argv)
{
    int num = 2;
    char disp = '*';
    char c;

    // TODO: 1. Use getopt() to parse the command-line arguments
    while ((c = getopt(argc, argv, "n:d:")) != -1)
        // TODO: 2. Use a switch statement to handle the different
        //           command-line arguments
        switch (c)
        {
        // TODO: 3. Handle the -n and -d arguments
        case 'n':
            num = atoi(optarg);
            break;

        case 'd':
            disp = optarg[0];
            break;

        // TODO: 4. Handle the default case
        default:
            usage();
            exit(1);
        }

    printf("number = %d, display char = %c\n",
           num, disp);

    // TODO: 5. Print the character n times
    for (int i = 0; i < num; i++)
        printf("%c\t", disp);
    printf("\n");

    return 0;
}

// Incase user does not provide the correct arguments
void usage(void)
{

    printf("Usage:\n");
    printf(" -n <repetitions>\n");
    printf(" -d <char to dispaly>\n");
    exit(8);
}
