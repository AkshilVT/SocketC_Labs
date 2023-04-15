/*------------------------------------
    * lab0.c
    *------------------------------------
    * This is a simple program that asks
    * the user for their name and then
    * greets them.
    *------------------------------------
    AUTHOR:  Akshil Thumar
    SEMESTER:  Fall 2023
------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // TODO: 1. Declare a character array of size 4096
    char buf[4096];
    printf("Hello World!\n");

    // TODO: 2. Ask the user for their name
    printf("Enter your name: ");
    scanf("%s", buf);

    // TODO: 3. Print out a greeting to the user
    printf("Hello %s\n", buf);
}