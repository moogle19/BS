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

    if(writeFile == NULL)
    {
        printf("Can't create output file! \n");
        return -1; //exit program
    }

    int bytes = 0; //size of file

    //MD5_CTX* c = (MD5_CTX*) malloc(16*1024);
    MD5_CTX c;

    //read until end of file
    printf("%s", "reading");
    int* input = malloc(sizeof(int));
    *input = fgetc(readFile);

    //unsigned char* hash = (unsigned char*) malloc(32*sizeof(unsigned char));
    unsigned char hash[16];

    MD5_Init(&c);




    while(*input != EOF)
    {
        bytes++;
        if(bytes%100000 == 0)
        {
            printf(".");
        }
        MD5_Update(&c, input, sizeof(unsigned char));
        fputc(*input, writeFile);
        *input = fgetc(readFile);
    }

    printf("\n%d", bytes);
    printf("%s\n", "Byte");

    MD5_Final(hash ,&c);

    int i = 0;
    for(i = 0;i < 16*sizeof(unsigned char); i+=sizeof(unsigned char))
    {
        //TODO
        printf("%02x", hash[i]); //fuehrende nullen mit %02 erzwingen
    }
    printf("\n");

    FILE *readWritten;
    
    MD5_CTX check;

    unsigned char controlhash[16];

    MD5_Init(&check);
    
    fflush(writeFile);
    fclose(writeFile);
    fflush(readFile);
    fclose(readFile);

    readWritten = fopen(argv[2], "rb");

    if(readWritten == NULL)
    {
        printf("%s\n", "Failed to open");
    }

    printf("%s\n", argv[2]);

    int* newinput = malloc(sizeof(int));
    *newinput = fgetc(readWritten);
    
    while(*newinput != EOF)
    {
        MD5_Update(&check, newinput, sizeof(unsigned char));
        *newinput = fgetc(readWritten);
    }

    MD5_Final(controlhash, &check);

    int j = 0;
    for(j = 0;j < 16*sizeof(unsigned char); j+=sizeof(unsigned char))
    {
        //TODO
        printf("%02x", controlhash[j]); //fuehrende nullen mit %02 erzwingen
    }
    printf("\n");

    //fflush(readWritten);
    fclose(readWritten);

    //free all the allocated space
    free(input);
    free(newinput);
    //free(hash);
    //free(c);
    //free(readFile);
    //free(writeFile);
    //free(readWritten);
    printf("%s\n", "end");

    return 0;
}
