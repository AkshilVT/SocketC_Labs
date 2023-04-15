#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buf[4096];
    printf("Hello World!\n");
    printf("Enter your name: ");
    scanf("%s", buf);
    printf("Hello %s\n", buf);
}