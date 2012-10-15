#include <stdio.h>
#include <openssl/md5.h>
#include "fcp.h"

int main(int argc, char **argv)
{
    FILE *readFile;
    FILE *writeFile;

    //checks if number of arguments is correct
    if(argc != 3)
    {
        printf("False number of Arguments!\n");
        return -1; //exit program
    }

    //opens file to copy
    readFile = fopen(argv[1], "r");

    //checks if fopen was succesful
    if(readFile == NULL)
    {
        printf("Failed to open file! \n");
        return -1; //exit program
    }

    //creates destionation file
    writeFile = fopen(argv[2], "w");

    if(writeFile == NULL)
    {
        printf("Can't create output file! \n");
        return -1; //exit program
    }

    int bytes = 0; //size of file

    //read until end of file
    printf("%s", "reading");
    int input = 0;
    input = fgetc(readFile);
    while(input != EOF)
    {
        bytes++;
        if(bytes%100000 == 0)
        {
            printf(".");
        }
        fputc(input, writeFile);
        input = fgetc(readFile);
    }

    printf("\n%d", bytes);
    printf("%s\n", "Byte");


    return 0;
}
