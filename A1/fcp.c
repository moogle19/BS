#include <stdio.h>
#include <stdlib.h>
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
    readFile = fopen(argv[1], "rb");

    //checks if fopen was succesful
    if(readFile == NULL)
    {
        printf("Failed to open file! \n");
        return -1; //exit program
    }

    //creates destionation file
    writeFile = fopen(argv[2], "wb");

    //checks if output file was created
    if(writeFile == NULL)
    {
        printf("Can't create output file! \n");
        return -1; //exit program
    }


    MD5_CTX c;

    int* input = malloc(sizeof(int));
    *input = fgetc(readFile);

    unsigned char hash[16];

    MD5_Init(&c);

    //read until end of file
    while(*input != EOF)
    {
        MD5_Update(&c, input, sizeof(unsigned char));
        fputc(*input, writeFile);
        *input = fgetc(readFile);
    }

    MD5_Final(hash ,&c);

    //print original hash
    printf("%s\t", "Original-Hash: ");
    int i = 0;
    for(i = 0;i < 16*sizeof(unsigned char); i+=sizeof(unsigned char))
    {
        printf("%02x", hash[i]); //fuehrende nullen mit %02 erzwingen
    }
    printf("\n");

    //flush and close write and read
    fflush(writeFile);
    fclose(writeFile);
    fflush(readFile);
    fclose(readFile);


    //check copyied file
    FILE *readWritten;
    MD5_CTX check;

    unsigned char controlhash[16];

    MD5_Init(&check);

    readWritten = fopen(argv[2], "rb");

    if(readWritten == NULL)
    {
        printf("%s\n", "Failed to open");
    }

    int* newinput = malloc(sizeof(int));
    *newinput = fgetc(readWritten);
    
    while(*newinput != EOF)
    {
        MD5_Update(&check, newinput, sizeof(unsigned char));
        *newinput = fgetc(readWritten);
    }

    MD5_Final(controlhash, &check);

    int hashcheck = 1;

    //print copy hash
    printf("%s\t", "Copy-Hash: ");
    for(i = 0;i < 16*sizeof(unsigned char); i+=sizeof(unsigned char))
    {
        //TODO
        printf("%02x", controlhash[i]); //fuehrende nullen mit %02 erzwingen
        if(hash[i] != controlhash[i])
        {
            hashcheck = 0;
        }
    }
    printf("\n");

    //flush and close
    fflush(readWritten);
    fclose(readWritten);

    //free all the allocated space
    free(input);
    free(newinput);

    //output success or failure
    if(hashcheck)
    {
        printf("%s\n", "MD5-Check:\tsuccesful\nCopying:\tsuccesful");
    }
    else
    {
        printf("%s\n", "MD5-Check:\tfailed\nCopying:\tfailed");
    }

    return 0;
}
